#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Fonctions_exo5.h"
#include "Version_Rapide.h"

void init_Zsg_ex5(Zsg *zone, int **M, int dim, int nbcl) {
    /*Initialisation des champs de la zone*/
    zone -> dim = dim;
    zone -> nbcl = nbcl;
    zone -> cell_som = NULL;
    zone -> bord_zsg =(Bordure_Graph *)malloc(sizeof(Bordure_Graph)* nbcl);
    /*Test malloc*/
   if((zone->bord_zsg)==NULL){
      fprintf(stderr, "Erreur malloc");
      exit(EXIT_FAILURE);
    }

    /*Initialisation des champs de Bordure_Graph*/
    for(int i=0; i<nbcl; i++) {

        (zone -> bord_zsg + i)->longu = 0;
        (zone -> bord_zsg + i)->liste_som = NULL;
    }
    /*Creation des graphes de la zone*/
    zone -> graph = (Graphe_zone *)malloc(sizeof(Graphe_zone));
    /*Test malloc*/
    if((zone->graph)==NULL){
      fprintf(stderr, "Erreur malloc");
      exit(EXIT_FAILURE);
    }
    *(zone -> graph) = cree_graphe_zone(M, dim);
}

void detruit_Graphe(Graphe_zone *graphe , int dim) {
    Cellule_som *temp;
    Cellule_som *cour;
    cour = graphe -> som;

    /* Liberation de la memoire */
    while (cour != NULL){
        temp = cour;

        /*Destruction du sommet cour*/
        detruit_liste(&(cour -> sommet -> cases));
        detruit_liste_sommet(&(cour -> sommet -> sommet_adj));
        free(cour -> sommet);

        cour = cour -> suiv;
        free(temp);
    }
    /* Destruction de la matrice */
    for(int i=0; i<dim; i++)
        free(graphe -> mat[i]);
    free(graphe -> mat);
}


void detruit_Zsg_ex5(Zsg *zone, int dim, int nbcl) {
    /*Destruction de la liste chainee des sommets de zone*/
    detruit_liste_sommet(&(zone -> cell_som));

    /*Destruction du tableau de listes de zone */
    for(int i=0; i<nbcl; i++){
      detruit_liste_sommet(&((zone -> bord_zsg) + i )-> liste_som);
    }

    free(zone -> bord_zsg);
    detruit_Graphe(zone -> graph, dim);
    free(zone -> graph);
    free(zone);
}


void change_couleur_zsg(Zsg *zone, int **M, int couleur) {


    Cellule_som *cour = zone -> cell_som;
    ListeCase liste = NULL;
    /* On parcourt la liste chainee des sommets de la zone */
    while(cour!=NULL) {
      /* On parcourt la liste des cases du sommet-zone */
      liste = cour -> sommet -> cases;
      /* Pour chaque case de la liste du sommet courant on lui attribut la couleur entree en parametre */
      while(liste!=NULL) {
        M[liste->i][liste->j] = couleur;
        liste = liste -> suiv;
        printf("[%d]\t",M[liste->i][liste->j]);
      }

      cour = cour -> suiv;

    }
}

/*Fonction qui renvoie vraie si la bordure est vide*/
int test_bordure_vide(Bordure_Graph *bordure, int nbcl) {

    for(int i=0; i<nbcl; i++){
        if(bordure[i].liste_som != NULL) /*On test si la liste est vide*/
            /*False si la bordure n'est pas vide*/
            return 0;
    }
    /*True sinon*/
    return 1;
}


/*Fonction qui renvoie la couleur la plus presente dans la bordure*/
int couleur_plus_presente(Bordure_Graph *bordure, int nbcl) {

    int couleur = -1; // Couleur n'a pas encore de valeur
    int nb_occ_max = 0;

    // On parcourt toutes les cases de chaque couleur de la bordure et on retourne la couleur presente dans la bordure qui possède le plus de case
    for(int i=0; i<nbcl; i++) {
        if(nb_occ_max < bordure[i].longu) {
            nb_occ_max = bordure[i].longu;
            couleur = bordure[i].liste_som -> sommet -> cl;
        }
    }
    return couleur;
}


/*Fonction de mise a jour de la Bordure_Graph, bascule une couleur de la bordure dans la zsg*/
void raffraichit_bord_graph(Zsg *zone, int couleur) {
    Sommet *s1;
    Cellule_som *case_cour;
    Cellule_som *liste_som_adj;
    /* Cases de la bordure de couleur "couleur" */
    case_cour=((zone -> bord_zsg + couleur) -> liste_som);
    /* Pour toutes les cases de couleur "couleur" on les ajoutent dans la Zsg */
    while(case_cour!=NULL) {

        ajoute_liste_sommet(&(zone -> cell_som), case_cour -> sommet);
        case_cour -> sommet -> marque = 0; // Car dans Zsg


        /* Pour tous les sommets adjacents a la case courante on met à jour la bordure de la zsg */
        liste_som_adj = case_cour -> sommet -> sommet_adj;
        while(liste_som_adj!=NULL) {
            s1 = liste_som_adj -> sommet;
            /* Cas ou le sommet n'a pas ete visite */
            if(s1 -> marque == 2) { // non present dans la bordure
                ajoute_liste_sommet(&((zone -> bord_zsg[s1 -> cl]).liste_som), s1);
                /* On ajoute le sommet a la bordure et on met a jour son statut */
                s1 -> marque = 1;
                // On incremente le nombre d'element present dans la bordure
                zone -> bord_zsg[s1->cl].longu = zone -> bord_zsg[s1->cl].longu + s1 -> nbcase_som;
            }
            liste_som_adj = liste_som_adj -> suiv;
        }
        case_cour = case_cour -> suiv;
    }
    /* Destruction de la liste */
    detruit_liste_sommet(&((zone -> bord_zsg[couleur]).liste_som));
    (zone -> bord_zsg[couleur]).longu = 0; // On remet a 0 le nombre d'element de cette couleur presente dans la bordure
}


/*Sequence de jeu avec max bordure*/
int max_bordure(int **M, Grille *grille, int dim, int nbcl, int aff) {
    int couleur;
    int cpt=0; // compteur d'essai
    Sommet *s1 = NULL;

    Zsg *zone = (Zsg *) malloc(sizeof(Zsg));
    /*Test malloc*/
    if (zone==NULL){
      fprintf(stderr, "Erreur malloc");
      exit(EXIT_FAILURE);
    }

    /* Initialisation de la zone */
    init_Zsg_ex5(zone, M, dim, nbcl);

    s1 = (zone->graph->mat)[0][0]; // On affecte la premiere case de la matrice au sommet s1
    /* Ajout du sommet a la liste chainee des sommets adjacents du tableau de pointeurs */
    ajoute_liste_sommet(&((zone->bord_zsg[s1->cl]).liste_som), s1);
    /* On incremente la taille de la bordure avec le nombre de case du sommet s1 */
    zone->bord_zsg[s1->cl].longu = zone->bord_zsg[s1->cl].longu + (s1->nbcase_som);

    /* Tant que la bordure de zone n'est pas vide, on recupere la couleur la plus presente dans la bordure et on ajoute toutes les cases de la bordure de cette couleur a la Zsg */
    while(!test_bordure_vide(zone->bord_zsg, nbcl)) {
        couleur=couleur_plus_presente((zone->bord_zsg), nbcl);
        raffraichit_bord_graph(zone, couleur);

        if(aff==1){
            for(int i=0; i<dim; i++) {
                for(int j=0; j<dim; j++){
                    if (zone->graph->mat[i][j]->marque != 0){
                        Grille_attribue_couleur_case(grille, i, j, zone->graph->mat[i][j]->cl);
                    }
                    else{
                        Grille_attribue_couleur_case(grille, i, j, couleur);
                    }
                }
            }
            Grille_redessine_Grille(grille);
            change_couleur_zsg(zone, M, couleur);
        }

        cpt++;
    }

    /*Liberation de la memoire*/
    detruit_Zsg_ex5(zone, dim, nbcl);
    return cpt-1;
}
