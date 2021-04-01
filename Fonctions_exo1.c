#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Entete_Fonctions.h"

void trouve_zone_rec(int **M, int nbcase, int i, int j, int *taille, ListeCase *L) {

  // initialisation de couleur avec la couleur de la case courante afin de garder cette couleur en memoire
  int couleur=M[i][j];

  // On teste si la case a deja ete visitee
  if(M[i][j]==-1)
  {
    return;
  }

  // Les couleurs correspondent ainsi, on affecte la valeur -1 a cette case et on ajoute la case (i,j) a la liste et on incremente la taille de la zone.
  M[i][j]=-1;
  ajoute_en_tete(L,i,j);
  (*taille)++;

  // Teste pour la case de gauche
  if(0<i) {
    // Teste si la case de gauche a la meme couleur que la case courante
    if(couleur==M[i-1][j])
      {
        // appel recursif sur la case a gauche de la case courante
        trouve_zone_rec(M, nbcase, i-1, j, taille, L);
      }
  }

  // Teste pour la case du dessus
  if(0<j) {
    // Teste si la case du dessus a la meme couleur que la case courante
    if(couleur==M[i][j-1])
    {
      // appel recursif sur la case au dessus de la case courante
      trouve_zone_rec(M, nbcase, i, j-1, taille, L);
    }
  }

  // Teste pour la case de droite
  if(i<nbcase-1) {
    // Teste si la case de droite a la meme couleur que la case courante
    if(couleur==M[i+1][j])
    {
      // appel recursif sur la case a droite de la case courante
      trouve_zone_rec(M, nbcase, i+1, j, taille, L);
    }
  }

  // Teste pour la case du dessous
  if(j<nbcase-1) {
    // Teste si la case du dessous a la meme couleur que la case courante
    if(couleur==M[i][j+1])
    {
      // appel recursif sur la case en dessous de la case courante
      trouve_zone_rec(M, nbcase, i, j+1, taille, L);
    }
  }
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff) {

  int cpt=0;
  int couleur; // couleur aleatoire
  int i,j;
  int taille_zsg=0; // taille de la zone superieure gauche

  // On alloue de la memoire pour la liste des cases de la zone
  ListeCase *liste=(ListeCase *)malloc(sizeof(ListeCase));

  // On teste si l'allocation a fonctionne
  if(liste==NULL)
  {
    puts("Erreur Malloc()");
    //exit(1);
  }

  // On creer une variable ListeCase afin de parcourir les elements de la liste
  ListeCase cours;

  // Tant que la zone superieure gauche ne contient pas toutes les cases de la grille
  while(taille_zsg!=(dim*dim))
  {

    init_liste(liste); // On initialise liste a la liste vide
    taille_zsg=0; // On affecte 0 a la taille de la liste

    if (aff==1){ /* Affichage de la grille */
    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	    Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }
    Grille_redessine_Grille(G);
    Grille_attente_touche();
    //sleep(1);  fait planter la fenetre au bout d'un certain nombre de tour de boucle
    }

    // On choisit une couleur aleatoirement jusqu'a ce que cette derniere soit differente de la couleur de la zone superieure gauche
    do {
        couleur=(int)rand()%nbcl;
    }while(couleur==M[0][0]);

    cpt++; // On incremente le compteur d'essais

    // On fait appel a la fonction trouve_zone_rec sur la case superieure gauche de la grille
    trouve_zone_rec(M, dim, 0, 0, &taille_zsg, liste);

    // On affecte a cours le premier element de liste
    cours=*liste;

    // Tant que cours ne vaut pas la valeur NULL
    while(cours)
    {
      M[cours->i][cours->j]=couleur; // On affecte les cases de la zone avec la couleur choisit aleatoirement
      cours=cours->suiv; // On avance dans la liste
    }
    detruit_liste(liste);
  }

  free(liste); // On libere l'espace memoire de la liste

  return cpt;
}


void trouve_zone_imp(int **M, int nbcase, int *taille, ListeCase *L){

  int i,j;

  // On alloue la memoire pour la pile de case
  ListeCase *liste=(ListeCase *)malloc(sizeof(ListeCase));
  init_liste(liste); // On initialise la liste

  // On ajoute a la liste la case superieure gauche
  ajoute_en_tete(liste, 0, 0);

  // On garde en memoire la couleur de la case superieure gauche
  int couleur=M[0][0];

  // Tant que la liste n'est pas vide
  while(!test_liste_vide(liste))
  {
    // On depile la liste
    enleve_en_tete(liste,&i, &j);

    // On teste si la case a deja ete visitee
    if(M[i][j]==-1)
    {
      continue;
    }

    // On teste si la case courante a la meme couleur que la zone superieure gauche
    if(M[i][j]==couleur)
    {
      // Les couleurs correspondent ainsi, on affecte la valeur -1 a cette case et on ajoute la case (i,j) a la liste et on incremente la taille de la zone.
      M[i][j]=-1;
      ajoute_en_tete(L,i,j);
      (*taille)++;
    }


    if(0<i) { // case gauche
      if(M[i][j]==-1) // On teste si la case courante a deja ete visitee
      {
        ajoute_en_tete(liste, i-1, j); // On empile
      }
    }

    if(0<j) { // case dessus
      if(M[i][j]==-1)
      {
        ajoute_en_tete(liste, i, j-1); // On empile
      }
    }

    if(i<nbcase-1) { // case droite
      if(M[i][j]==-1)
      {
        ajoute_en_tete(liste, i+1, j); // On empile
      }
    }

    if(j<nbcase-1) { // case dessous
      if(M[i][j]==-1)
      {
        ajoute_en_tete(liste, i, j+1); // On empile
      }
    }
  }

  free(liste); // On libere la memoire

}


int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff) {

  int cpt=0;
  int couleur; // Couleur aleatoire
  int i,j;
  int taille_zsg=0; // Taille de la zone superieure gauche

  // On alloue de la memoire pour la liste des cases de la zone
  ListeCase *liste=(ListeCase *)malloc(sizeof(ListeCase));

  // On teste l'allocation
  if(liste==NULL)
  {
    puts("Erreur Malloc()");
    //exit(1);
  }

  // On creer une variable ListeCase afin de parcourir les elements de la liste
  ListeCase cours;

  // Tant que la zone superieure gauche ne contient pas toutes les cases de la grille
  while(taille_zsg!=(dim*dim))
  {

    init_liste(liste); // On initialise la liste a la liste vide
    taille_zsg=0; // On affecte 0 a la taille de la liste

    if (aff==1){ /* Affichage de la grille */
      for (i=0;i<dim;i++)
        for (j=0;j<dim;j++){
          Grille_attribue_couleur_case(G,i,j,M[i][j]);
        }
      Grille_redessine_Grille(G);
      //Grille_attente_touche();
      sleep(1);  //fait planter la fenetre au bout d'un certain nombre de tour de boucle
    }

    // On choisit une couleur aleatoirement jusqu'a ce que cette derniere soit differente de la couleur de la zone superieure gauche
    do {
        couleur=(int)rand()%nbcl;
    }while(couleur==M[0][0]);

    cpt++; // On incremente le compteur d'essais

    // On fait appel a la fonction trouve_zone_imp
    trouve_zone_imp(M, dim, &taille_zsg, liste);

    // On affecte a cours le premier element de liste
    cours=*liste;

    // Tant que cours ne vaut pas la valeur NULL
    while(cours)
    {
      // On affecte les cases de la zone avec la couleur choisit aleatoirement
      M[cours->i][cours->j]=couleur;
      cours=cours->suiv; // On avance dans la liste
    }
    detruit_liste(liste);
  }

  free(liste); // On libere l'espace memoire de la liste

  return cpt;
}
