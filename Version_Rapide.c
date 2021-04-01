#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Entete_Fonctions.h"

void init_Zsg(S_Zsg *zone_b, int dim, int nbcl)
{
    int i,j;

    // Initialisation des champs de la structure zone_b
    zone_b->dim=dim;
    zone_b->nbcl=nbcl;

    // Initialisation de la liste des cases de la zone superieure a la valeur NULL
    init_liste(&(zone_b->Lzsg));

    // On alloue de la memoire pour le tableau de liste de case de la bordure (tableau B de nbcl cases)
    zone_b->B=(ListeCase *)malloc(sizeof(ListeCase)*nbcl);

    // On teste l'allocation
    if(zone_b->B==NULL) {
        fprintf(stderr,"Erreur Malloc");
        exit(1);
    }

    // On initialise les cases du tableau B a la valeur NULL
    for(i=0; i<nbcl; i++) {
        init_liste(zone_b->B+i);
    }

    // On alloue de la memoire pour le tableau a double entree des appartenances, il s'agit d'une matrice dim*dim
    zone_b->App=(int **)malloc(sizeof(int *)*dim);

    // On teste l'allocation
    if(zone_b->App==NULL) {
        fprintf(stderr,"Erreur Malloc");
        exit(1);
    }


    for(i=0; i<dim; i++) {
        // De cette maniere on alloue de l'espace memoire de la taille dim pour chaque colonne du tableau
        zone_b->App[i]=(int *)malloc(sizeof(int)*dim);
        // On teste l'allocation
        if((zone_b->App[i])==NULL) {
            fprintf(stderr,"Erreur Malloc");
            exit(1);
        }
        // Initialisation de chaque case a la valeur -2, case non visitee donc n'appartenant pas a Lzsg ni a la bordure
        for(j=0; j<dim; j++) {
            zone_b->App[i][j]=-2;
        }
    }

}


int ajoute_Zsg(S_Zsg *zone_b, int i, int j)
{
    // On affecte la valeur -1 a la case car appartient a Lzsg
    zone_b->App[i][j]=-1;
    // Ajout de la case dans la liste des cases de la zone superieure gauche
    return ajoute_en_tete(&(zone_b->Lzsg), i, j);
}


int ajoute_Bordure(S_Zsg *zone_b, int i, int j, int couleur)
{
    // On affecte la valeur de la couleur donnee en parametre correspondant a la couleur de la bordure
    zone_b->App[i][j]=couleur;
    // Ajout de la case dans le tableau de listes de cases de la bordure reparties par couleurs
    return ajoute_en_tete(&(zone_b->B[couleur]), i, j);
}


int appartient_Zsg(S_Zsg *zone_b, int i, int j)
{
    // Renvoie vrai si la case est dans Lzsg, donc de valeur -1
    return zone_b->App[i][j]==-1;
}


int appartient_Bordure(S_Zsg *zone_b, int i, int j, int couleur)
{
    // Renvoie vrai si une case est dans la bordure de couleur donnee, donc de valeur couleur
    return zone_b->App[i][j]==couleur;
}

void detruit_Zone(S_Zsg *zone_b)
{
    int i;

    /* On detruit la liste des cases de la zone Zsg */
    detruit_liste(&zone_b->Lzsg);

    /* Le tableau ayant nbcl listes de cases, on parcourt tous les indices jusqu'a nbcl-1 et on detruit toutes les listes de cases */
    for(i=0; i<zone_b->nbcl; i++) {
        detruit_liste(zone_b->B+i);
    }
    free(zone_b->B); // On libère la mémoire allouee pour le tableau

    for(i=0; i<zone_b->dim; i++) { // On libere la memoire allouer pour toutes les lignes du tableau a double entree App
        free(zone_b->App[i]);
    }
    free(zone_b->App); // On libere la memoire allouee pour le tableau entier

    free(zone_b); // On libere la memoire allouee pour la S_Zsg
}

int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l)
{
    // On alloue de la memoire
    ListeCase *liste=(ListeCase *)malloc(sizeof(ListeCase));

    // On teste l'allocation
    if(liste==NULL)
    {
        fprintf(stderr,"Erreur Malloc");
        exit(1);
    }

    int nb_case=0; // nb de cases ajoutee

    init_liste(liste); // Initialisation a la valeur NULL

    ajoute_en_tete(liste, k, l); // Ajout de la case (k,l) a la liste

    int couleur;

    // Tant que la liste n'est pas vide
    while(!test_liste_vide(liste))
    {
        enleve_en_tete(liste, &k, &l); // On depile
        couleur=M[k][l];  //On recupere la couleur de la case courante

        // Si la couleur de la case est differente de la couleur de la zone, c'est qu'elle fait partie de la bordure
        if(couleur!=cl)
        {
            // Donc si elle n'est pas dans la bordure on l'ajoute dans la liste qui gere les cases de la bordure
            if(!appartient_Bordure(Z, k, l, couleur))
            {
                ajoute_Bordure(Z, k, l, couleur);
            }
            continue; // Sinon on sort de la condition
        }

        // Dans le cas : couleur de la case meme que couleur de la zone
        if(couleur==cl)
        {
            // Mais case pas encore dans la liste qui gere les cases de la zone
            if(!appartient_Zsg(Z, k, l))
            {
                // Alors on l'ajoute et on incremente le nombre de case
                ajoute_Zsg(Z, k, l);
                nb_case++;
            }
        }

        // Traitement des cases adjacentes, on teste leur appartenance a la zone superieure gauche et a la bordure
        if(0<k) // case de gauche
        {
            if(!appartient_Bordure(Z, k-1, l, M[k-1][l])) { //
              if(!appartient_Zsg(Z, k-1, l)) {
                  ajoute_en_tete(liste, k-1, l);
              }
            }
        }

        if(0<l) // case du dessus
        {
            if(!appartient_Bordure(Z, k, l-1, M[k][l-1])) {
              if(!appartient_Zsg(Z, k, l-1)) {
                ajoute_en_tete(liste, k, l-1);
              }
            }
        }

        if(k<Z->dim-1) // case de droite
        {
            if(!appartient_Bordure(Z, k+1, l, M[k+1][l])) {
              if(!appartient_Zsg(Z, k+1, l)) {
                ajoute_en_tete(liste, k+1, l);
              }
            }
        }

        if(l<Z->dim-1) // case du dessous
        {
            if(!appartient_Bordure(Z, k, l+1, M[k][l+1])) {
              if(!appartient_Zsg(Z, k, l+1)) {
                ajoute_en_tete(liste, k, l+1);
              }
            }
        }
    }

    free(liste); // On libere la memoire

    return nb_case;
}


int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff)
{
    int cpt=0; // compteur d'essais
    int couleur=M[0][0]; // Couleur case superieure gauche
    int i,j;
    int taille_zsg=0; // Taille de la zone superieure gauche

    // On alloue de la memoire pour une structure S_Zsg
    S_Zsg *zone_b=(S_Zsg *)malloc(sizeof(S_Zsg));

    // Test malloc
    if (zone_b==NULL){
        puts("Erreur malloc");
        exit(1);
    }

    // Initialisation de tous les champs de la structure
    init_Zsg(zone_b, dim, nbcl);
    ListeCase cours;
    ListeCase *liste_b = NULL;
    /* On affecte le nombre de cases qui on ete ajoute a la Zsg a la varible taille_zsg */
    taille_zsg = agrandit_Zsg(M, zone_b, M[0][0], 0, 0);


    while(taille_zsg != dim*dim) { /* Tant que la Zsg ne recouvre pas toute la grille, c'est a dire que le jeu n'est pas termine */
        /* On recupere dans "cours" la liste des cases de la Zsg */
        cours=zone_b->Lzsg;

        // On choisit une couleur aleatoirement jusqu'a ce que cette derniere soit differente de la couleur de la zone superieure gauche
        do {
            couleur=(int)(rand()%nbcl);
        } while(couleur==M[0][0]);

        /* On applique la couleur "couleur" a toutes les cases de la Lzsg, c'est a dire a toutes les cases de la zsg*/
        while(cours!=NULL){
            M[cours->i][cours->j]=couleur;
            cours=cours->suiv;
        }

        if (aff==1){ /* Affichage de la grille */
            for (i=0;i<dim;i++)
                for (j=0;j<dim;j++){
                    Grille_attribue_couleur_case(G,i,j,M[i][j]);
                }
                Grille_redessine_Grille(G);
                //Grille_attente_touche();
                sleep(1);
        }

        cpt++; // On incremente le compteur d'essais

        /* On recupere les cases de la bordure */
        liste_b=zone_b->B;

        /* On ajoute a la Zsg les cases de la bordure qui ont la meme couleur que la Zsg et a chaque qu'on ajoute une case a la Zsg, on la retire de la bordure*/
        while(liste_b[couleur]!=NULL) {
            enleve_en_tete(&liste_b[couleur], &i, &j);
            taille_zsg = taille_zsg + agrandit_Zsg(M, zone_b, couleur, i, j); // On incremente le nombre de case de la Zsg (important pour la condition de fin de jeu!)
        }
    }

    detruit_Zone(zone_b); // A la fin du jeu, on detruit la zone

    return cpt+1; // on retourne le nombre d'essai
}

/*  QUESTION 3  */
/*  Plus la dimension augmente plus le nombre d'essais augmente donc le temps est allonge
*   Au bout d'une certaine dimension, le programme declenche une erreur
*   On remarque aussi que plus le nombre de couleur est eleve, plus le nombre d'essais est tres eleve.
*   A 10 couleurs differentes nous arrivons deja a 400 essais.
*   La vitesse du programme est plus influencee par le nombre de couleurs que par la dimension.
*   Exemple: cas ou dim=50 et couleur=5 --> temps et nombre d'essais seront beaucoup moins eleves que dans le cas ou dim=5 couleur=50
*/
