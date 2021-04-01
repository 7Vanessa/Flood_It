#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"
#include "Version_Rapide.h"
 
 
int main(int argc,char**argv)
{

  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 
 
  if(argc!=7){
    printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-1-2> <aff 0/1>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);
  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Allocation puis generation de l'instance */

  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);


  if (aff==1){ /* Affichage de la grille */
    Grille_init(dim,nbcl, 500,&G);

    Grille_ouvre_fenetre(G);

    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	    Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }

    Grille_redessine_Grille(G);
    //Grille_attente_touche();
  }

  temps_initial = clock ();
  

  if (exo==0){
     printf("%d essais\n", sequence_aleatoire_rec(M, G, dim, nbcl, aff)); 
  }

  if (exo==1){
     printf("%d essais\n", sequence_aleatoire_imp(M, G, dim, nbcl, aff)); 
  }

  if (exo==2){
     printf("%d essais\n", sequence_aleatoire_rapide(M, G, dim, nbcl, aff)); 
  }
  
  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
  printf("%f\n",temps_cpu);
 
 /* Desallocation de la matrice */
  for(i = 0; i< dim; i++) {
    if (M[i])
        free(M[i]);
  }
  if (M) free(M);

  
  if (aff==1){ /* Fermeture et désallocation de la grille */
    Grille_ferme_fenetre();

    Grille_free(&G);
  }

  return 0;
}
