#ifndef __VERSION_RAPIDE__
#define __VERSION_RAPIDE__

#include "Entete_Fonctions.h"

// Initialisation de la structure
void init_Zsg(S_Zsg *zone_b, int dim, int nbcl);

// Ajoute une case dans la liste Lzsg
int ajoute_Zsg(S_Zsg *zone_b, int i, int j);

// Ajoute une case dans la bordure d'une couleur donnee
int ajoute_Bordure(S_Zsg *zone_b, int i, int j, int couleur);

// Renvoie vrai si une case est dans Lzsg
int appartient_Zsg(S_Zsg *zone_b, int i, int j);

// Renvoie vrai si une case est dans la bordure de couleur donnee
int appartient_Bordure(S_Zsg *zone_b, int i, int j, int couleur);

// Met a jour les champs Lzsg et B d'une S_Zsg lorsqu'une case k,l de couleur cl, qui est dans la bordure B[cl] doit basculer dans la Lzsg. Et retourne le nombre de case qui a ete ajoute a Lzsg
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

// Algorithme tirant au sort une couleur: il utilise la fonction agrandit_zone pour determiner la Zsg
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);

// Detruit tous les elements
void detruit_Zone(S_Zsg *zone_b);

#endif  /* __VERSION_RAPIDE__ */
