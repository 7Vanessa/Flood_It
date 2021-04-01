#ifndef __FONCTIONS_PARCOURS__
#define __FONCTIONS_PARCOURS__

#include "Graphe_zone.h"

typedef struct queue{
	Cellule_som *tete;
	Cellule_som *dernier;
} Queue;

/*Initialisation de file*/
void init_Queue(Queue **file);

/*Test file vide*/
int is_empty_queue(Queue **file);

/*Enfiler un sommet, ajout en fin de file*/
void enqueue_queue(Queue **file, Sommet *s);

/*Enlever element en tete de file et renvoyer le premier element de la file*/
Sommet *dequeue_queue(Queue **file);

/*Effacer toute la file*/
void clear_queue(Queue **file);

/*Parcours en largeur du Graphe_zone graphe du Sommet racine au Sommet fin en nb d'arcs minimum*/
void parcours_largeur(Graphe_zone *graphe, Sommet *racine, Sommet *fin);

/*Fonction permettant de lancer une sequence de jeu et renvoyant le nombre d'essais */
int sequence_jeu_parcours(int **M, Grille *G, int dim, int nbcl, int aff);


#endif
