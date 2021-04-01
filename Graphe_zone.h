#ifndef __Graphe_ZONE__
#define __Graphe_ZONE__

#include "Entete_Fonctions.h"
#include "Liste_case.h"

typedef struct sommet Sommet;

/* Element d'une liste chainee de pointeurs sur Sommets */
typedef struct cellule_som{
  Sommet * sommet;
  struct cellule_som * suiv;
} Cellule_som;

struct sommet {
  int marque; /* Statut du sommet (0 si dans Zsg, 1 si dans bordure, 2 si non visite) */
  int distance; /* Nb d'arretes reliant ce sommet a la racine du parcours en largeur */
  Sommet *pere; /* Pere du sommet dans l'arborescence du parcours en largeur */
  int num; /* Numero du sommet (sert uniquement a l'affichage) */
  int cl; /* Couleur d'origine du sommet-zone */
  ListeCase cases; /* Listes de cases du sommet-zone */
  int nbcase_som; /* Nombre de cases de cette liste */

  Cellule_som * sommet_adj; /* Liste des arretes pointeurs sur les sommets adjacents */
};

typedef struct graphe_zone {
  int nbsom; /* Nombre de sommets dans le graphe */
  Cellule_som *som; /* Liste chainee des sommets du graphe */
  Sommet ***mat; /* Matrice de pointeurs sur les sommets indiquant a quel sommet appartient une case (i,j) de la grille */
} Graphe_zone;

/* Ajoute un pointeur sur sommet  a une liste chainee Cellule_som passee en parametre */
void ajoute_liste_sommet(Cellule_som **L, Sommet *sommet);

/* Detruit une liste chainee de Cellule_som sans detruire les sommets pointees par cette liste */
void detruit_liste_sommet(Cellule_som **L);

/* Met a jour 2 sommets s1 et s2 en indiquant qu'ils sont adjacents l'un a l'autre */
void ajoute_voisin(Sommet *s1, Sommet *s2);

/* Renvoie vrai si et seulement si 2 sommets sont adjacents */
int adjacent(Sommet *s1, Sommet *s2);

/* Fonction permettant de creer le graphe tout entier */
Graphe_zone cree_graphe_zone(int **M, int dim);

/* Affichage du graphe */
void affiche_graphe(Graphe_zone graphe);

#endif
