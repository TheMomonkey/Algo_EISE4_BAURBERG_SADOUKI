#include <stdio.h>
#include <stdlib.h>
#include "algo_danger1.h"

/* TODO :
	- ajouter option "verbose" a specifier dans Makefile
*/

int main(int argc, char const *argv[])
{

/*-------------------------INIT------------------------------*/	

	int pi[] = {0,4,6,8,9,12,14,17,18};
	sommet_t * tete_liste_danger = NULL;
	sommet_t * a_placer = NULL;

	printf("AVANT INIT :\n");
	for (int i = 0; i < NB_SOMMETS; i++) printf("pi[%d] = %02d --> sommet numero %d\n", i, pi[i], i+1);
	
	for (int i = 0; i < NB_SOMMETS-1; i++) {
		a_placer = (sommet_t*)(malloc(sizeof(sommet_t)));
		tete_liste_danger = place_danger(pi[i], pi[i+1], i+1, tete_liste_danger, a_placer);
	}
	a_placer = (sommet_t*)(malloc(sizeof(sommet_t)));
	tete_liste_danger = place_danger(pi[NB_SOMMETS-1], NB_ARETES*2, NB_SOMMETS, tete_liste_danger, a_placer);

	printf("APRES INIT :\n");
	print_liste(tete_liste_danger);

/*-------------------------ALGO------------------------------*/	

	sommet_t * plus_dangereux_non_place = tete_liste_danger;
	int** matrice = (int**)(malloc(N*sizeof(int*)));
	for (size_t i = 0; i < N; i++)
	{
		matrice[i] = (int*)(malloc(N*sizeof(int)));
	}
	int milieu = (N-1)/2;
	remplir_matrice(matrice,0,N-1,milieu,plus_dangereux_non_place);

/*-------------------------FREE------------------------------*/	

	free_liste(tete_liste_danger);
	for (size_t i = 0; i < N; i++)
	{
		free(matrice[i]);
	}
	free(matrice);

    return 0;
}
