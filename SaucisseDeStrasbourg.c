#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NB_LIGNES 6
#define NB_COLONNES 7

typedef int Grille[NB_LIGNES][NB_COLONNES];

int main(int argc, char** argv);
void chargerGrille(char** argv, Grille grille);

const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;

/*
* VOTRE STRATEGIE
*/
int chercherLigne(Grille laGrille, int col){
    int ligne = -1;

    while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
        ligne++;
    }
    return ligne;
}

void IA2(Grille laGrille, char pion, int * ligne, int * colonne ){
    void testerPion(int * j0, int * j1, int * j2, int pion, int nous) {
        if (pion == nous) { *j1 = *j1 + 1; }
        else if (pion != VIDE) { *j2 = *j2 + 1; }
        else { *j0 = *j0 + 1; }
    }
    void checkerAlignements(Grille laGrille, int * importance, int dejaImp, int ligne, int colonne, int pion, bool compterAlignDeux) {
        int j0, j1, j2;
        int j, k;
        int val = 0;
        *importance = 0;
        j0 = 0;
        j1 = 0;
        j2 = 0;
        if (ligne < 4 && compterAlignDeux != true) { // pcq compterAlignDeux c'est pour le check du pion au dessus, donc on a pas besoin de tester ce qu'il y a en dessous
            // 
            for (j = ligne + 1; j < ligne + 3; j++) {
                testerPion(&j0, &j1, &j2, laGrille[j][colonne], pion);
            }
            val = j1 - j2;
            // 
            if (j1 == 2 || j2 == 2) {
                if (j < NB_LIGNES) { testerPion(&j0, &j1, &j2, laGrille[j][colonne], pion); } else { val--; } // tester pour voir si il y a un troisième pion
            }
            
            if (j1 > 2 || (j2 > 2 && dejaImp != 6)) { *importance = 6;  }
            else if (j2 == 2 && ligne < 0 && dejaImp < 4 && *importance < 4) { *importance = 4;  }
            else if (ligne > (2 - val)) { *importance = 2;  } // fix verti ?
        }
        if (*importance != 6) {
            for (j = -1; j < 2 && *importance != 3; j++) {
                if (colonne + j > 0 && colonne + j < NB_COLONNES - 1) {
                    // 
                    val = 0;
                    j0 = 0;
                    j1 = 0;
                    j2 = 0;
                    for (k = j - 1; k < j + 2; k++) {
                        testerPion(&j0, &j1, &j2, laGrille[ligne][colonne + k], pion);
                    }
                    if (j0 + j1 == 3) {
                        val++;
                    }
                    // 
                    if (colonne + j - 2 >= 0) { testerPion(&j0, &j1, &j2, laGrille[ligne][colonne + j - 2], pion); } else { val--; }
                    if (colonne + k < NB_COLONNES) { testerPion(&j0, &j1, &j2, laGrille[ligne][colonne + k], pion); } else { val--; }
                    
                    val = val + j0 + j1 - j2;

                    if (j1 > 2 || (j2 > 2 && dejaImp != 6)) { *importance = 6;  }
                    else if (val == 6 && j1 > 1 && dejaImp < 5 && *importance < 5) { *importance = 5;  }
                    else if (val == 1 && j2 == 2 && j1 == 0 && dejaImp < 4 && *importance < 4) { *importance = 4;  }
                    else if (val == 6 && j1 == 1 && dejaImp < 3 && *importance < 3) { *importance = 3;  }
                    else if (val == 6 && dejaImp < 2 && *importance < 2) { *importance = 2;  }
                    else if (val == 4 && dejaImp == 0 && *importance == 0) { *importance = 1;  }

                    // if (j2 > 2 || j1 > 2) { *importance = 3;  }
                    // else if (*importance < 2 && (j2 > 1 && j2 + j0 > 3)) { *importance = 2;  }
                    // if ((j1 + j0 > 3 && *importance == 0) || ((j1 > 0 && j1 + j0 > 2) && *importance < 2)) { *importance = 1;  } //ne marche pas pcq nouvelle methode consiste à n'en tester que 2 (si on teste 3 à gauche par exemple, ça peut détecter même si pas collé à la colonne actuelle)
                }
            }
            if (*importance != 6) {
                for (j = -1; j < 2 && *importance != 3; j++) {
                    if (ligne + j > 0 && ligne + j < NB_LIGNES - 1 && colonne + j > 0 && colonne + j < NB_COLONNES - 1) {
                        // 
                        val = 0;
                        j0 = 0;
                        j1 = 0;
                        j2 = 0;
                        for (k = j - 1; k < j + 2; k++) {
                            testerPion(&j0, &j1, &j2, laGrille[ligne + k][colonne + k], pion);
                        }
                        // 
                        if (j0 + j1 == 3) {
                            val++;
                        }
                        if (ligne + (j - 2) >= 0 && colonne + (j - 2) >= 0) { testerPion(&j0, &j1, &j2, laGrille[ligne + (j - 2)][colonne + (j - 2)], pion); } else { val--; }
                        if (ligne + k < NB_LIGNES && colonne + k < NB_COLONNES) { testerPion(&j0, &j1, &j2, laGrille[ligne + k][colonne + k], pion); } else { val--; }
                        
                        val = val + j0 + j1 - j2;

                        if (j1 > 2 || (j2 > 2 && dejaImp != 6)) { *importance = 6;  }
                        else if (val == 6 && j1 > 1 && dejaImp < 5 && *importance < 5) { *importance = 5;  }
                        else if (val == 1 && j2 == 2 && dejaImp < 4 && *importance < 4) { *importance = 4;  }
                        else if (val == 6 && j1 == 1 && dejaImp < 3 && *importance < 3) { *importance = 3;  }
                        else if (val == 6 && dejaImp < 2 && *importance < 2) { *importance = 2;  }
                        else if (val == 4 && dejaImp == 0 && *importance == 0) { *importance = 1;  }

                        // if (j2 > 2 || j1 > 2) { *importance = 3;  }
                        // else if (*importance < 2 && (j2 > 1 && j2 + j0 > 3)) { *importance = 2;  }
                        // if ((j1 + j0 > 3 && *importance == 0) || ((j1 > 0 && j1 + j0 > 2) && *importance < 2)) { *importance = 1;  } //ne marche pas pcq nouvelle methode consiste à n'en tester que 2 (si on teste 3 à gauche par exemple, ça peut détecter même si pas collé à la colonne actuelle)
                    }
                }
                if (*importance != 6) {
                    for (j = -1; j < 2 && *importance != 3; j++) {
                        if (ligne - j > 0 && ligne - j < NB_LIGNES - 1 && colonne + j > 0 && colonne + j < NB_COLONNES - 1) {
                            // 
                            val = 0;
                            j0 = 0;
                            j1 = 0;
                            j2 = 0;
                            for (k = j - 1; k < j + 2; k++) {
                                testerPion(&j0, &j1, &j2, laGrille[ligne - k][colonne + k], pion);
                            }
                            // 
                            if (j0 + j1 == 3) {
                                val++;
                            }
                            if (ligne - (j - 2) < NB_LIGNES && colonne + (j - 2) >= 0) { testerPion(&j0, &j1, &j2, laGrille[ligne - (j - 2)][colonne + (j - 2)], pion); } else { val--; }
                            if (ligne - k >= 0 && colonne + k < NB_COLONNES) { testerPion(&j0, &j1, &j2, laGrille[ligne - k][colonne + k], pion); } else { val--; }
                            
                            val = val + j0 + j1 - j2;

                            if (j1 > 2 || (j2 > 2 && dejaImp != 6)) { *importance = 6;  }
                            else if (val == 6 && j1 > 1 && dejaImp < 5 && *importance < 5) { *importance = 5;  }
                            else if (val == 1 && j2 == 2 && dejaImp < 4 && *importance < 4) { *importance = 4;  }
                            else if (val == 6 && j1 == 1 && dejaImp < 3 && *importance < 3) { *importance = 3;  }
                            else if (val == 6 && dejaImp < 2 && *importance < 2) { *importance = 2;  }
                            else if (val == 4 && dejaImp == 0 && *importance == 0) { *importance = 1;  }

                            // if (j2 > 2 || j1 > 2) { *importance = 3;  }
                            // else if (*importance < 2 && (j2 > 1 && j2 + j0 > 3)) { *importance = 2;  }
                            // if ((j1 + j0 > 3 && *importance == 0) || ((j1 > 0 && j1 + j0 > 2) && *importance < 2)) { *importance = 1;  } //ne marche pas pcq nouvelle methode consiste à n'en tester que 2 (si on teste 3 à gauche par exemple, ça peut détecter même si pas collé à la colonne actuelle)
                        }
                    }
                }
            }
        }
    }
    int i;
    int tours = 0;
    int colonneUtile = 8;
    *ligne = 8;
    *colonne = 8;
    int ligneDeColonne;
    int importance;
    int dejaImp = 0;
    int dejaDang = 6;
    int dangerosite;
    int cases[NB_COLONNES][2];
    i = 3;
    while (*ligne == 8 && (i != 3 || (colonneUtile == 8 && tours != 2))) { // il fait les 3 tours même si colonne != 8 et ça fait chier mais il se démerde
        importance = 0;
        dangerosite = 6;
        ligneDeColonne = chercherLigne(laGrille, i);
        if (i == 3) { tours++; };
        if (ligneDeColonne != -1) {
            if (tours == 1) {
                checkerAlignements(laGrille, &importance, dejaImp, ligneDeColonne, i, pion, false);
                cases[i][0] = importance;
                if (ligneDeColonne != 0) {
                    checkerAlignements(laGrille, &dangerosite, 0, ligneDeColonne - 1, i, pion, true);
                } else {
                    dangerosite = 0;
                }
                cases[i][1] = dangerosite;
            } else {
                importance = cases[i][0];
                dangerosite = cases[i][1];
            }
            if (importance > dejaImp || (importance == dejaImp && dangerosite < dejaDang)) {
                *colonne = i;
                dejaImp = importance;
                dejaDang = dangerosite;
            }
            // if (importance == 2 && *colonne == 8) {
            //     // printf("Test de dangerosité\n");
            //     if (dangerosite < (2 + (tours == 2))) { // ou !=3 ? ou on fait en fonction du tour ?
            //          // printf("Deux !\n");
            //         *colonne = i;
            //         if (importance != 0) {
            //             dejaImp = importance;
            //         }
            //     }
            // } else if ((importance == 1 || ligneDeColonne > 2) && colonneUtile == 8) { // GROS PROB SA MERE : si pos < 4 MAIS il y a suffisament de pions alliés en dessous, il ignore quand même
            //     if (ligneDeColonne != 0) {
            //         // printf("Test de dangerosité\n");
            //         if (dangerosite < (2 + (tours == 2))) { // ou !=3 ? ou on fait en fonction du tour ?
            //             // printf("Utile\n");
            //             colonneUtile = i;
            //         }
            //     } 
            // }
        } else {
            cases[i][0] = -1;
        }
        // printf("%d %d - I=%d D=%d\n", ligneDeColonne, i, importance, dangerosite);
        i--;
        if (i < 0) { i = 6; }
        // i = i % NB_COLONNES; // pour revenir à 0 quand on dépasse les bornes
    }
    // printf("Je veux jouer en colonne %d\n", *colonne);
    if (*colonne == 8) { *colonne = colonneUtile; }
    if (*colonne == 8) {
        // printf("Au secours !\n");
        int rescueIndex = 0;
        int rescue;
        for (rescue = 1; rescue < NB_COLONNES; rescue++) {
            if (cases[rescue][0] > cases[rescueIndex][0] || ((cases[rescue][0] == cases[rescueIndex][0]) && (cases[rescue][1] < cases[rescueIndex][1]))) {
                rescueIndex = rescue;
            }
        }
        *colonne = rescueIndex;
    }
    // printf("Je VAIS jouer en colonne %d\n", *colonne);
    if (*ligne == 8) { *ligne = chercherLigne(laGrille, *colonne); }
}

// La fonction principale reçoit la grille du tour et retourne le coup choisi
// Vous n'avez pas à modifier cette fonction
int main(int argc, char** argv) 
{
    Grille grille;
    int laLigne = 0;
    int laColonne = 0;
    chargerGrille(argv, grille);

    IA2(grille, JOUEUR, &laLigne, &laColonne);

    return laColonne;
}

// Charge la grille du tour actuel
// Vous n'avez pas à modifier cette fonction
void chargerGrille(char** argv, Grille grille) 
{
    for(int i = 0; i < NB_LIGNES; i++)
        for(int j = 0; j < NB_COLONNES; j++)
            grille[i][j] = atoi(argv[i * NB_COLONNES + j + 1]);
}
