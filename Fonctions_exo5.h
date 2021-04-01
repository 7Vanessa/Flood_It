#ifndef __FONCTIONS_EXO5__
#define __FONCTIONS_EXO5__

#include "Graphe_zone.h"
#include "API_Grille.h"


typedef struct Bordure_Graph {
    int longu; /* Taille de la liste chainee des elements de meme couleur */
    Cellule_som *liste_som;  /* Liste chainee des sommets adjacents du graphes de la même couleur */
} Bordure_Graph;

typedef struct Zsg {
    int dim; /* dimension de la grille */
    int nbcl; /* nombres de couleurs */
    Cellule_som *cell_som; /* Sommet de la zone */
    Bordure_Graph *bord_zsg; /*Tableau de listes de pointeurs sur sommet*/

    Graphe_zone *graph; /* Graphe des sommets */
} Zsg;


/*Initialisation des champs de la zone*/
void init_Zsg_ex5(Zsg *zone, int **M, int dim, int nbcl);


/* Detruit le Graphe_zone */
void detruit_Graphe(Graphe_zone *graphe , int dim);


/* Détruit la Zsg */
void detruit_Zsg_ex5(Zsg *zone, int dim, int nbcl);


/* Fonction de changement de couleur*/
void change_couleur_zsg(Zsg *zone, int **M, int couleur);


/*Fonction qui renvoie vraie si la bordure est vide*/
int test_bordure_vide(Bordure_Graph *bordure, int nbcl);


/*Fonction qui renvoie la couleur la plus presente dans la bordure*/
int couleur_plus_presente(Bordure_Graph *bordure, int nbcl);


/*Fonction de mise a jour de la Bordure_Graph, bascule une couleur de la bordure dans la zsg*/
void raffraichit_bord_graph(Zsg *zone, int couleur);


/*Sequence de jeu avec max bordure*/
int max_bordure(int **M, Grille *grille, int dim, int nbcl, int aff);


#endif
