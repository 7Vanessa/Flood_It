#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fonctions_parcours.h"
#include "Fonctions_exo5.h"
#include "Graphe_zone.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"

#define GRAND_NOMBRE 9999999

/*Fonction qui initialise la file*/
void init_Queue(Queue **file){
	(*file)->tete=NULL;
	(*file)->dernier=NULL;
}

/*Fonction qui renvoie vrai si la file est vide*/
int is_empty_queue(Queue **file){
	return (*file)->tete==NULL;
}

/*Fonction qui ajoute un sommet s en fin de file*/
void enqueue_queue(Queue **file, Sommet *s){
	Cellule_som *new = (Cellule_som*)malloc(sizeof(Cellule_som));
	new->sommet = s;
	new->suiv = NULL;

	/*Test malloc*/
	if (new == NULL){
		fprintf(stderr, "Erreur malloc");
		exit(EXIT_FAILURE);
	}

	/*si la file est vide, alors le premier element de la file est egal a new*/
	if(is_empty_queue(file)){
		(*file)->tete = new;
	}else{ /*sinon, le dernier element de la file pointe vers new et donc on enfile*/
		(*file)->dernier->suiv=new;
	}
	(*file)->dernier=new;/*et donc le dernier element de la file est maintenant egal a new*/
}

/*Fonction qui defile la file et qui renvoie le premier element de la file */
Sommet *dequeue_queue(Queue **file){
	Cellule_som *first = (*file)->tete;
	Sommet *s = first->sommet;

	if(is_empty_queue(file)){
		printf("File vide, rien a retourner");
		return NULL;
	}
	/*Si il n'y a pas qu'un seul element*/
	if(first->suiv != NULL){
		(*file)->tete = first->suiv;
	}else{
		(*file)->tete = NULL;
		(*file)->dernier = NULL;
	}
	free(first);
	return s;
}

/*Fonction qui efface toute la file*/
void clear_queue(Queue **file){
	if (is_empty_queue(file)){
		printf("File vide");
		return;
	}
	while(!(is_empty_queue(file))){
		dequeue_queue(file);
	}
}

/*Parcours en largeur du graphe du sommet racine au sommet fin*/
void parcours_largeur(Graphe_zone *graphe, Sommet *racine, Sommet *fin){
	/*Initialisation des champs du sommet fin avec GRAND_NOMBRE car il n'est pas encore visite et pere a NULL car on le connait pas encore*/
	fin->distance=GRAND_NOMBRE;
	fin->pere=NULL;

	/*Utilisation d'une file pour trouver la bordure du parcours en largeur*/
	Queue *file=(Queue*)malloc(sizeof(Queue));
	init_Queue(&file);

	/*La distance de la racine a la racine du parcours vaut 0 puis on enfile cette racine a notre file precedemment initialisee*/
	racine->distance=0;
	enqueue_queue(&file, racine);

	/*Utilisation de sommets qui vont permettre d'enfiler et de defiler la file du parcours, liste_ajd est la liste d'adjacence de s1*/
	Sommet *s1, *s2;
	Cellule_som *liste_adj;
	while (!(is_empty_queue(&file))){
		/*s2 est le premier sommet de la file apres qu'elle ait ete defilee*/
		s1=dequeue_queue(&file);
		liste_adj=s1->sommet_adj;

		while(liste_adj!=NULL){
			/*A chaque tour de boucle s2 est egal a un sommet de la liste d'adjacence de s1*/
			s2=liste_adj->sommet;

			/*Si le sommet n'est pas encore visité*/
			if (s2->distance == GRAND_NOMBRE){
				s2->distance = s1->distance +1;
				/*Le pere de s2 est egal a s1*/
				s2->pere=s1;
				/*Puis on enfile s2 dans notre file*/
				enqueue_queue(&file, s2);

			}
			/*Si v est deja egal au sommet final on n'a pas besoin de file on le detruit et on sort de la fonction car il n'y a rien a parcourir*/
			if(s2==fin){
				clear_queue(&file);
				return;
			}
			liste_adj=liste_adj->suiv;
		}

	}

}

int sequence_jeu_parcours(int **M, Grille *grille, int dim, int nbcl, int aff) {

	int cpt=0;
	int couleur1, couleur2;

	Zsg *zone = (Zsg *) malloc(sizeof(Zsg));
	/*Test malloc*/
	if (zone==NULL){
		fprintf(stderr, "Erreur malloc");
		exit(EXIT_FAILURE);
	}
	init_Zsg_ex5(zone, M, dim, nbcl);
	Graphe_zone *graphe = zone->graph;
	Sommet *racine = NULL;
	Sommet *fin = NULL;

	/*La racine du graphe est la zone superieure gauche et la fin est la zone inferieure droite*/
	racine=(zone->graph)->mat[0][0];
	fin=(zone->graph)->mat[dim-1][dim-1];

	/*Ajout de racine a la liste chainee des sommets adjacents de zone et de la meme couleur que racine*/
	ajoute_liste_sommet(&((zone->bord_zsg[racine->cl]).liste_som),racine);
	/*Incrémentation de la longueur de la liste chainee des elements de meme couleurs avec le nombre de cases de sommets*/
  	zone->bord_zsg[racine->cl].longu= zone->bord_zsg[racine->cl].longu + racine->nbcase_som;
  	/*On rafraichit le graphe avec la couleur de racine*/
  	raffraichit_bord_graph(zone, racine->cl);

  	/*On effectue le parcours en largeur de racine vers fin*/
  	parcours_largeur(graphe, racine, fin);

	/*Enregistrement du parcours du plus cours chemin*/
	Cellule_som *parcours=NULL;
	Sommet *cour=fin;
	/*Ajout du sommet cour a la liste du parcours*/
	while(cour!=racine){
		ajoute_liste_sommet(&parcours, cour);
		/*On remonte de la fin du parcours vers le pere*/
		cour=cour->pere;
	}

	/*On parcourt la liste du parcours en largeur pour inonder le plus court chemin de la zone superieure gauche a la zone inferieure droite*/
	while(parcours!=NULL){

		couleur1=parcours->sommet->cl;
		raffraichit_bord_graph(zone, couleur1);

		if(aff==1){
			for(int i=0; i<dim; i++) {
					for(int j=0; j<dim; j++){
						/*Si la case n'est pas dans la zone*/
							if (zone->graph->mat[i][j]->marque != 0){
									Grille_attribue_couleur_case(grille, i, j, zone->graph->mat[i][j]->cl);
							}
							else{
									Grille_attribue_couleur_case(grille, i, j, couleur1);
							}
					}
			}
			Grille_redessine_Grille(grille);
    		change_couleur_zsg(zone, M, couleur1);
    	}
    /*On passe au sommet suivant de la liste du parcours en largeur*/
    parcours=parcours->suiv;
    cpt++;
	}

/*Utilisation de la boucle de la fonction max_bordure*/
	while(!test_bordure_vide(zone->bord_zsg, nbcl)) {

			couleur2=couleur_plus_presente(zone->bord_zsg, nbcl);
			raffraichit_bord_graph(zone, couleur2);

			if(aff==1){
				for(int i=0; i<dim; i++) {
						for(int j=0; j<dim; j++){
							/*Si la case n'est pas dans la zone*/
								if (zone->graph->mat[i][j]->marque != 0){
										Grille_attribue_couleur_case(grille, i, j, zone->graph->mat[i][j]->cl);
								}
								else{
										Grille_attribue_couleur_case(grille, i, j, couleur2);
								}
						}
				}
				Grille_redessine_Grille(grille);
				change_couleur_zsg(zone, M, couleur2);
			}
			cpt++;
	}
	/*On libere la liste et la zone supérieure gauche*/
	detruit_liste_sommet(&parcours);
	detruit_Zsg_ex5(zone, dim, nbcl);
	return cpt;
}
