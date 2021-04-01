all: Flood-It_Partie1 Flood-It_Partie2

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1.c

Version_Rapide.o: Version_Rapide.c Version_Rapide.h Entete_Fonctions.h Liste_case.h
	gcc -c Version_Rapide.c

Graphe_zone.o: Graphe_zone.c Graphe_zone.h Entete_Fonctions.h Liste_case.h 
	gcc -c Graphe_zone.c

Fonctions_exo5.o: Fonctions_exo5.c Fonctions_exo5.h Graphe_zone.h
	gcc -c Fonctions_exo5.c

fonctions_parcours.o: fonctions_parcours.c fonctions_parcours.h Entete_Fonctions.h Fonctions_exo5.h Graphe_zone.h
	gcc -c fonctions_parcours.c

Flood-It_Partie1.o: Flood-It_Partie1.c
	gcc -c Flood-It_Partie1.c

Flood-It_Partie1: Flood-It_Partie1.o Liste_case.o  API_Grille.o API_Gene_instance.o Fonctions_exo1.o Version_Rapide.o
	gcc -o Flood-It_Partie1 Flood-It_Partie1.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Version_Rapide.o -I include -L lib -lmingw32 -lSDL2main -lSDL2

Flood-It_Partie2.o: Flood-It_Partie2.c
	gcc -c Flood-It_Partie2.c

Flood-It_Partie2: Flood-It_Partie2.o Liste_case.o  API_Grille.o API_Gene_instance.o Graphe_zone.o Fonctions_exo5.o fonctions_parcours.o
	gcc -o Flood-It_Partie2 Flood-It_Partie2.o Liste_case.o API_Grille.o API_Gene_instance.o Graphe_zone.o Fonctions_exo1.o Fonctions_exo5.o fonctions_parcours.o -I include -L lib -lmingw32 -lSDL2main -lSDL2

clean:
	rm -f *.o Flood-It_Partie1 Flood-It_Partie2
