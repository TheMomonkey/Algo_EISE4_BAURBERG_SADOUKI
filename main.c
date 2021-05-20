#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algo_danger1.h"
#include "instanciation.h"

/* TODO :
	- ajouter option "verbose" a specifier dans Makefile
*/

extern int dangerosite, nb_sommets,nb_voisins_max,cpt,voisins_tot;
int N = 0;

int main(int argc, char const *argv[])
{

/*-------------------------INIT------------------------------*/	

	int *pi;
	int *alpha;
	int nb_voisins_tot = instanciation(argc,argv,&pi,&alpha);
	printf("nb_voisins_tot = %d\n",nb_voisins_tot);

/*	int pi[] = {0,4,6,8,9,12,14,17,18};
	int alpha[] = {3,5,2,9,1,6,1,5,7,3,1,7,7,2,4,6,5,9,8,1};
*/
	sommet_algo1_t * tete_liste_danger = NULL;
	sommet_algo1_t * a_placer = NULL;

	printf("AVANT INIT :\n");
	for (int i = 0; i < nb_sommets; i++) printf("pi[%d] = %02d --> sommet numero %d\n", i, pi[i], i+1);
	
	for (int i = 0; i < nb_sommets-1; i++) {
		a_placer = (sommet_algo1_t*)(malloc(sizeof(sommet_algo1_t)));
		tete_liste_danger = place_danger(pi[i], pi[i+1], i+1, tete_liste_danger, a_placer);
	}
	a_placer = (sommet_algo1_t*)(malloc(sizeof(sommet_algo1_t)));
	tete_liste_danger = place_danger(pi[nb_sommets-1], nb_voisins_tot, nb_sommets, tete_liste_danger, a_placer);

	printf("APRES INIT :\n");
	print_liste(tete_liste_danger);

/*-------------------------ALGO------------------------------*/	

	N = sqrt(nb_sommets);
	sommet_algo1_t * plus_dangereux_non_place = tete_liste_danger;
	int** matrice = (int**)(malloc(N*sizeof(int*)));
	for (size_t i = 0; i < N; i++)
	{
		matrice[i] = (int*)(malloc(N*sizeof(int)));
		for (size_t j = 0; j < N; j++)
		{
			matrice[i][j] = 0;
		}
		
	}
	
	int milieu = (N - 1)/2;
	remplir_matrice(matrice,pi,alpha,0,N-1,milieu,plus_dangereux_non_place);
/*
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			matrice[i][j] = i*N + j + 1;
		}
		
	}
	int lig = 1;
	int col = 5;
	int voisins[8][2] = {0}; // tableau des coordonnees des voisins
	int nb_voisins = get_voisins(voisins, lig, col); // remplit "voisins" et renvoie nb cases remplies
	printf("(%d,%d)",voisins[0][0],voisins[0][1]);
	for (size_t i = 1; i < 8; i++)
	{
		printf(",(%d,%d)",voisins[i][0],voisins[i][1]);
	}
*/
	

	if (argc > 1) fprint_matrice(matrice, argv[1]);
	else 		  fprint_matrice(matrice, "matrice.txt");

/*-------------------------FREE------------------------------*/	

	free_liste(tete_liste_danger);
	
	for (size_t i = 0; i < N; i++)
	{
		free(matrice[i]);
	}
	free(matrice);
	
    return 0;
}
