#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Graphe_zone.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"

#define GRAND_NOMBRE 9999999

void ajoute_liste_sommet(Cellule_som **L, Sommet *s) {
  Cellule_som *temp = (Cellule_som *)malloc(sizeof(Cellule_som)); /* pointeur pour parcourir L */

  if(temp==NULL) { /* test le malloc */
    fprintf(stderr, "Erreur malloc");
    exit(EXIT_FAILURE);
  }
  temp -> sommet = s; /* Ajoute le sommet a la liste */
  temp -> suiv = *L; /* On attache le nouveau maillon temp a la chaine L */
  *L=temp; /* Et on fait pointer le debut de la liste sur temp */
}

void detruit_liste_sommet(Cellule_som **L) {
    Cellule_som *cour,*temp; /* Creer 2 pointeurs pour parcourir la liste chainee */
    cour=*L; /* On fait pointer cour sur le premier element de la chaine */
    while (cour!=NULL){ /* On avance dans la chaine et on supprimme chaque element de cette liste  */
      temp=cour;
      temp -> sommet = NULL;
      cour=cour->suiv;
      free(temp);
   }
   *L=NULL;
}


void ajoute_voisin(Sommet *s1, Sommet *s2) {
  ajoute_liste_sommet(&(s1->sommet_adj), s2); /* On ajoute le sommet s2 a liste des sommets adjacent a s1 */
  ajoute_liste_sommet(&(s2->sommet_adj), s1); /* On ajoute le sommet s1 a liste des sommets adjacent a s2 */
}


int adjacent(Sommet *s1, Sommet *s2) {
  Cellule_som *cour; /* Creer 2 pointeurs pour parcourir la liste chainee */
  cour=s1->sommet_adj; /* On fait pointer cour sur le premier element de la chaine */

  while (cour!=NULL){ /* On avance dans la chaine et on verifie si s2 se trouve dans la liste de sommet adjacent a s1 */
     if(cour->sommet==s2) { /* Si s2 s'y trouve on retourne vrai */
       return 1;
     }
     cour=cour->suiv;
  }
  return 0; /* Sinon on retourne faux */
}

/* On creer le graphe a partir d'une matrice et d'une dimension entrees en parametre */
Graphe_zone cree_graphe_zone(int **M, int dim) {
  int taille; // compteur de cases presentes dans une zone
  int nbsommet=1; // compteur de sommet crees
  int i,j;
  Graphe_zone graphe;

  /* Initialisation des champs du graphe */

  graphe.nbsom = 0;
  graphe.som = NULL;

  /* On initalise la matrice du graphe */
  graphe.mat=(Sommet ***)malloc(sizeof(Sommet **) *dim); /* On lui alloue de la memoire */
  for(i=0; i<dim; i++) {
    (graphe.mat)[i]=(Sommet **)malloc(sizeof(Sommet *) * dim); /* Allocation en mémoire pour chaque ligne de la matrice */
    for(j=0; j<dim; j++) {
      graphe.mat[i][j]=NULL; /* On initialise a NULL chaque case de la matrice */
    }
  }

  /* Premier parcours de la matrice : creation des sommets du graphe */
  for(i=0; i<dim; i++) {
    for(j=0; j<dim; j++) {
      if(graphe.mat[i][j]==NULL) { /* On test si la case de la grille n'a pas encore de sommet assigné */

        taille = 0; // On remet a 0 la varible taille pour chque creation d'un nouveau sommet

        /* On alloue donc a chaque fois un nouveau sommet en memoire */
        Sommet *sommet=(Sommet *) malloc(sizeof(Sommet));

        /* On initialise ensuite tous les champs de ce nouveau sommet */
        sommet -> marque = 2; // Le sommet n'a pas encore ete visite, donc 2
        sommet -> distance = GRAND_NOMBRE;
        sommet -> pere = NULL;
        sommet -> num = nbsommet; // On attribut la valeur de nbsommet au champs num de sommet, il s'agit du numero de sommet
        sommet -> cl = M[i][j]; // On attribue au champs cl du sommet la couleur de la case correspondant se trouvant dans la matrice entree en parametre de la fonction
        sommet -> cases = NULL; // On initialise au debut a NULL la liste des cases de la zone pointee par ce sommet
        sommet -> nbcase_som = 0; // "cases" etant NULL, on initialise a 0
        sommet -> sommet_adj = NULL; // On initialise a NULL la liste des sommets des zones adcentes a la zone representee par ce sommet

        nbsommet++; // On incremente le compteur de sommet a chaque creation d'un nouveau sommet

        ajoute_liste_sommet(&graphe.som, sommet); // On ajoute le sommet creer a la liste chainee des sommets du graphe

        /* On definie la zone dans laquelle ce situe la case courante i,j, on met donc a jour la liste des cases "cases" du sommet-zone et on met aussi a jour la variable "taille" indiquant le nombre de cases contenu dans cette zone */
        trouve_zone_rec(M, dim, i, j, &taille, &(sommet->cases));

        /* On met a jour le champs nbcase_som du sommet a l'aide du compteur de cases de la zone "taille" */
        sommet -> nbcase_som = taille;

        /* Pour chaque case presente dans la liste des cases du sommet-zone, on attibut a la matrice du graphe le sommet correspondant  */
        while(sommet->cases!=NULL) {
          graphe.mat[sommet->cases->i][sommet->cases->j] = sommet;
          sommet->cases = sommet->cases -> suiv;
        }
        graphe.nbsom++; // On incremente ensuite le champs du graphe representant le nombre de sommet present dans le graphe
      }
    }
  }

  /* Deuxieme parcours de la matrice : creation des arretes du graphe */
  for(i=0; i<dim; i++) {
    for(j=0; j<dim; j++) {
      /* Pour chaque case on teste si les cases adjacentes a la case courante se trouve dans une autre zone ou non */
      if(i<dim-1) {
        if(graphe.mat[i][j]!=graphe.mat[i+1][j]) { // On teste si le sommet de la case courante est identique ou non au sommet de la case de droite
          if(adjacent(graphe.mat[i][j], graphe.mat[i+1][j])==0) { // On teste si les 2 sommets sont adjacent ou non
            /* Ainsi si les 2 cases sont dans 2 zones differentes et si leur sommets ne sont pas deja adjacent, creation de l'arrete en ajoutant le sommet de la case courante a la liste des sommets adjacent a la case de droite et reciproquement */
            ajoute_voisin(graphe.mat[i][j], graphe.mat[i+1][j]);
          }
        }
      }
      /* Meme chose que precedement mais pour la case du dessous a la case courante */
      if(j<dim-1) {
        if(graphe.mat[i][j]!=graphe.mat[i][j+1]) {
          if(adjacent(graphe.mat[i][j], graphe.mat[i][j+1])==0) {
            ajoute_voisin(graphe.mat[i][j], graphe.mat[i][j+1]);
          }
        }
      }
    }

  }
return graphe;
}


void affiche_graphe(Graphe_zone graphe) {

  Sommet *sommet;
  Cellule_som *elem = graphe.som;

  printf("graphe : %d sommets et idem pour le nombre de zones\n", graphe.nbsom);

  while(elem!=NULL) {
    /* On print tous les sommets du graphe */
    sommet = elem -> sommet;
    printf("Le sommet s%d est de couleur %d et a %d cases.\n", sommet->num, sommet->cl, sommet->sommet_adj);
    Cellule_som *L_som_adj = sommet->sommet_adj;
    printf("Ces voisins sont :\n");
    while(L_som_adj!=NULL) {
      printf("\tSommet s%d\t", L_som_adj->sommet->num);
      L_som_adj = L_som_adj->suiv;
    }
    printf("\n");
    elem = elem -> suiv;
  }

}
