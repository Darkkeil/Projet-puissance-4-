
/**
* @file puissance_4.c
* @brief un puissance 4
* @author Youen Minaud
* @version finale
* @date 18/11/2022
*
* Implémentation d'un puissance 4 en langage c
* en suivant une maquette fait au préalable
* dans le cadre de la SAE 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const char PION_A = 'X'; //pion du joueur A
const char PION_B = 'O'; //pion du joueur B
const char VIDE = ' '; // valeur d'une case vide
const char INCONNU = ' '; //  il n'y a pas de vainqueur en debut de partie
const char CURSEUR = '^'; // curseur lors de l'affichage de la grille
const int REJOUER = 1; // choix positif pour renouveller une partie
#define NBLIG 6 // nombre de lignes 
#define NBCOL 7 // nomber de colones 
const int COLONNE_DEBUT = NBCOL/2; // la colonne de base de l'affichage 

typedef char grille[NBLIG][NBCOL]; // deinition d'un type grille format grille de puissance 4

// fonction + prodecure 
void init_grille(grille);
void afficher(grille,char,int);
bool grille_pleine(grille);
int choisir_colonne(grille,char,int);
int trouverligne(grille,int);
bool gagnant_ligne(grille,int,int);
bool gagnant_colonne(grille,int,int);
bool gagnant_diag_gauche(grille,int,int);
bool gagnant_diag_droite(grille,int,int);
bool estVainqueur(grille,int,int);
bool rejouer();
void finDePartie(char,char,char);
void jouer(grille,char,int *,int *);
int main()
{
    bool nouvelle_partie = true; // intialisé à true pour lancer la première partie
    int victoires_consécutives_joueur1 = 0;
    int victoires_consécutives_joueur2 = 0;
    while (nouvelle_partie) //tant que on veut jouer 
    {
    char vainqueur,joueur1,joueur2,gagnant_precedant;
    int ligne,colonne,nb_partie; 
    nb_partie = 0;
    grille tab;
    init_grille(tab); // initialisation de la grille
    gagnant_precedant = vainqueur;
    vainqueur = INCONNU; // vainqueur encore inconnu
    if (nb_partie == 0)
    {
        gagnant_precedant = INCONNU; // c'est la première partie pas de gagnant précédant donc
    }
    srand(time(NULL));
    int ordre_jeu = rand()%2+1;
    /** determine qui sera le premier joueur aléatoirement **/
    if (ordre_jeu == 2)
    {
        joueur1 = PION_B;
        joueur2 = PION_A;
    }
    else
    {
        joueur2 = PION_A;
        joueur1 = PION_B;
    }
    printf("le joueur du pion %c commence !\n",joueur1);
    afficher(tab,joueur1,COLONNE_DEBUT); // affichage de la grille avant le premier
    while ((vainqueur == INCONNU) && (!grille_pleine(tab))) // on joue tant que la grille n'est pas pleine et que personne n'a gagné
    {
        jouer(tab,joueur1,&ligne,&colonne);
        afficher(tab,joueur2,COLONNE_DEBUT);
        if (estVainqueur(tab,ligne,colonne))
        {
            vainqueur=joueur1;
            victoires_consécutives_joueur1=victoires_consécutives_joueur1+1;
        }
        else if (! grille_pleine(tab))
        {
            jouer(tab,joueur2,&ligne,&colonne);
            afficher(tab,joueur1,COLONNE_DEBUT);
            if (estVainqueur(tab,ligne,colonne))
            {
                vainqueur=joueur2;
                victoires_consécutives_joueur2=victoires_consécutives_joueur2+1;
            }
        }
    }
    /** pour determiner le nombre de victoires consécutives impossible car le joueur 1 n'a pas toujours le même pion de parties e t parties **/
    finDePartie(vainqueur,joueur1,joueur2);
    if ((gagnant_precedant == joueur1)&&(vainqueur == joueur1))
    {
        printf("joueur 1 vous avez %d victoires consécutives!",victoires_consécutives_joueur1);
        victoires_consécutives_joueur2 = 0;
    }
    else if ((gagnant_precedant == joueur2)&&(vainqueur == joueur2))
    {
        printf("joueur 2 vous avez %d victoires consécutives!",victoires_consécutives_joueur2);
        victoires_consécutives_joueur1 = 0;
    }
    nb_partie++;
    nouvelle_partie = rejouer();
    }
    return EXIT_SUCCESS;  
}
bool estVainqueur(grille tab,int lig,int col) // determine si un joueur a gagné
{
    bool gagn;
    gagn = false;
    if (gagnant_ligne(tab,lig,col))
    {
        gagn = true;
    }
    else if (gagnant_colonne(tab,lig,col))
    {
        gagn = true;
    }
    else if (gagnant_diag_gauche(tab,lig,col))
    {
        gagn = true;
    }
    else if (gagnant_diag_droite(tab,lig,col))
    {
        gagn = true;
    }
    return gagn;
}
void jouer(grille tab,char pion,int*ligne,int*colonne)
    {
        *colonne = choisir_colonne(tab,pion,COLONNE_DEBUT);
        *ligne = trouverligne(tab,*colonne);
        while (*ligne == -1) // tant que la colonne où on veut jouer est pleine on doit saisir une autre colonne 
        {
            *colonne = choisir_colonne(tab,pion,*colonne);
            *ligne = trouverligne(tab,*colonne);
        }
        tab[*ligne][*colonne] = pion;  // remplissage de la case où le pion est tombé
    }
void init_grille(grille tableau)
{
    int i,j;
    for(i = 0;i < NBLIG;i++)
    {
        for(j = 0;j < NBCOL;j++)
        {
            tableau[i][j] = VIDE;
        }
    }
}
void afficher(grille tableau,char pion,int num_col)
{
    int i,j,curseur;
    char joueur;
    bool premier_coup = true;
    if (pion == PION_A) // determine quel joueur est le pion du joueur qui joue
    {
        joueur = 'A';
    }
    else
    {
        joueur = 'B';
    }
    curseur = 0; //nombre d'espace avant le curseur de la colonne où on joue
     /** o, verifie que ce ne soit pas le premier affichage ou personne ne joue afin de ne pas afficher le messagee après un coup **/
    i=0;
    while(i < NBLIG && premier_coup)
    {
        j = 0;
        while(j < NBCOL && premier_coup)
        {
            if(tableau[i][j] != VIDE)
            {
                premier_coup = false;
            }
            j++;    
        }
        i++;
    }
    if (!premier_coup)
    {
        system("clear"); //efface le terminal
    }
    if(num_col == 0)
    {
        printf("\n   %c\n",pion); //décalage de deux car il n'y a pas de colonnes avant la colonne une
        printf("   ^\n");
    }
    else{
        curseur = curseur+4;
        for (j = 0;j < num_col;j++){ //j=1 car on sait qu'on n'a pas joué en colonne une !
            curseur = curseur+5;
            if (j == num_col-1)
            {
                for (j = 0;j < curseur;j++)
                {
                    printf(" ");
                }
                printf("%c\n",pion);
                for (j = 0;j < curseur;j++)
                {
                    printf(" ");
                }
                printf("%c\n",CURSEUR);
                
            }
            else{
                curseur++;
            }
        }
    }
    for(i = 0;i < NBLIG;i++)
    {
        if (i == 0)
        {
            printf("___________________________________________\n");
        }
        else{
            printf("-------------------------------------------\n");
        }
        printf("|");
        for(j = 0;j < NBCOL;j++)
        {
            printf("  %c  |",tableau[i][j]);
        }
        printf("\n");  
    }
    printf("___________________________________________\n");
    if (!premier_coup)
    {
        printf("joueur %c à joué en colonne %d\n",joueur,num_col);
    }
}

bool grille_pleine(grille tableau)
{
    int i, j;
    bool plein = true; // de base on considère la grille pleine
    i = 0;
    while(i < NBLIG && plein)
    {
        j = 0;
        while(j < NBCOL && plein)
        {
            if(tableau[i][j] == VIDE) 
            {
                plein = false; // mais si on trouve uen case vide elle est forcément pas pleine!
            }
            j++;    
        }
        i++;
    }
    return plein;
}


int choisir_colonne(grille tableau ,char pion,int num_init)
{
    /** algo selon maquette **/
    int colonne_actu;
    printf("jouez votre pion %c dans une colonne non pleine où vous voulez jouer:\n",pion);
    scanf("%d",&colonne_actu);
    colonne_actu--;
    while (colonne_actu>NBCOL-1 || colonne_actu<0) // tant qu'on joue dans uen colonne invalide
    {
        if (colonne_actu>NBCOL-1 || colonne_actu<0) // si on rejoue dans une colonne invalide
        {
            printf("vous jouez dans une colonne inexistante!\nVous ne pouvez pas faire celà!\n");
        }
        printf("Vous ne pouvez pas jouer dans une colonne pleine!\nVeuillez jouer dans une colonne non pleine entre 1 et 7:\n");
        scanf("%d",&colonne_actu);
        colonne_actu--;
    }
    return colonne_actu;
}

int trouverligne(grille tableau,int colonne)
{
    int ii,ligne;
    ligne =-1;
    ii=0;
    while ((tableau[ii][colonne] == VIDE) && ii < NBLIG) // si la case sous le pion qui tombe est vide et que le pion n'est pas en ligne 6
    {
        ii++;
    }
    if (ii>=0)
    {
        ligne = ii-1;
    }
    return ligne;  
}
/** pour la suite sur les 4 fonctions de detection de victoire on aura le même fonctionnement**/
/** on regarde le nombre de voisins consécutifs de gauche du pion dont on test la victoire**/
/** de même pour les voisins de droite**/
/** si les voisins de gauche + voisins droite + 1 = 4 alors il y a victoire du joueur possésseur du pion**/
/**on additionne 1 car on compte le pion lui même dans l'alignement de 4**/
bool gagnant_ligne(grille tableau,int ligne,int colonne)
{
    int i,voisins_gauche,voisins_droite;
    bool gagnant = false;
    voisins_gauche = 0;
    voisins_droite = 0;
    for(i = colonne-1;i >= 0;i--)
    {
        if (tableau[ligne][i] == tableau[ligne][colonne])
        {
            voisins_gauche++;
        }
        else
        {
            break;
        }

    }
    for(i = colonne+1;i < NBCOL;i++)
    {
        if (tableau[ligne][i] == tableau[ligne][colonne])
        {
             voisins_droite++;
        }
        else
        {
            break;
        }
    }
    if ((voisins_gauche+voisins_droite+1) >= 4 )
    {
        gagnant = true;
    }
    return gagnant;
}
bool gagnant_colonne(grille tableau,int ligne,int colonne)
{
    int i,voisins_gauche,voisins_droite;
    bool gagnant = false;
    voisins_gauche = 0;
    voisins_droite = 0;
    for(i = ligne-1;i >= 0;i--)
    {
        if (tableau[i][colonne]==tableau[ligne][colonne])
        {
            voisins_gauche++;
        }
        else
        {
            break;
        }

    }
    for(i = ligne+1;i < NBLIG;i++)
    {
        if (tableau[i][colonne] == tableau[ligne][colonne])
        {
             voisins_droite++;
        }
        else
        {
            break;
        }
    }
    if ((voisins_gauche+voisins_droite+1) >= 4 )
    {
        gagnant = true;
    }
    return gagnant;
}
bool gagnant_diag_droite(grille tableau,int ligne,int colonne)
{
    int i,voisins_gauche,voisins_droite,cpt;
    bool gagnant = false;
    voisins_gauche = 0;
    voisins_droite = 0;
    cpt = 1; // on ne regarde  pas le pion qu'on test donc on regarde directement le premier pion à sa gauche
    for(i = ligne+1;i < NBLIG;i++)
    {
        /** pour chaque voisins suivant à gauche on augmente de ligne et on diminue en colonne de 1 dans la grille **/
        if (tableau[ligne+cpt][colonne-cpt] == tableau[ligne][colonne])
        {
            voisins_gauche++;
        }
        else
        {
            break;
        }
        cpt++;

    }
    cpt = 1; // on ne regarde  pas le pion qu'on test donc on regarde directement le premier pion à sa droite
    for(i = ligne-1;i >= 0;i--)
    {
        /** pour chaque voisins suivant à droite on diminue de ligne et on augmente en colonne de 1 dans la grille **/
        if (tableau[ligne-cpt][colonne+cpt]==tableau[ligne][colonne])
        {
             voisins_droite++;
        }
        else
        {
           break;
        }
        cpt++;
    }
    if ((voisins_gauche+voisins_droite+1) == 4)
    {
        gagnant = true;
    }
    return gagnant;
}
bool gagnant_diag_gauche(grille tableau,int ligne,int colonne)
{
    int i,voisins_gauche,voisins_droite,cpt;
    bool gagnant = false;
    voisins_gauche = 0;
    voisins_droite = 0;
    cpt = 1; // on ne regarde  pas le pion qu'on test donc on regarde directement le premier pion à sa droite
    for(i = ligne-1;i >= 0;i--)
    {
        /** pour chaque voisins suivant à gauche on diminue de ligne et on diminue en colonne de 1 dans la grille **/
        if (tableau[ligne-cpt][colonne-cpt] == tableau[ligne][colonne])
        {
            voisins_gauche++;
        }
        else
        {
            break;
        }
        cpt++;

    }
    cpt = 1; // on ne regarde  pas le pion qu'on test donc on regarde directement le premier pion à sa droite
    for(i = ligne+1;i < NBLIG;i++)
    {
        /** pour chaque voisins suivant à droite on augmente de ligne et on augmente en colonne de 1 dans la grille **/
        if (tableau[ligne+cpt][colonne+cpt] == tableau[ligne][colonne])
        {
             voisins_droite++;
        }
        else
        {
            break;
        }
        cpt++;
    }
    if ((voisins_gauche+voisins_droite+1) >= 4)
    {
        gagnant = true;
    }
    return gagnant;
}
void finDePartie(char pion_gagnant,char pion_joueur_1,char pion_joueur_2)
{
    if (pion_gagnant == pion_joueur_1)
    {
        printf("le joueur 1 à gagné la partie!\n");

    }
    else if (pion_gagnant == pion_joueur_2)
    {
        printf("le joueur 2 à gagné la partie!\n");
    }
    else
    {
        printf("Quels joueurs! Match nul!\n");
    }
}

bool rejouer()
{
   bool ok;
   int choix;
   printf("Souhaitez vous rejouer(1) ou arrêtez(0) ?\n");
   scanf("%d",&choix);
   if (choix == REJOUER)
   {
    ok = true;
   }
   else
   {
    ok = false;
   }
   return ok;
}


