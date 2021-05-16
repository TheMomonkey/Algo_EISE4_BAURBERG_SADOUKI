#include <stdio.h>
#include <stdlib.h>
#include "algo_danger1.h"

/* Affichage d'une liste grace a sa tete 
	--> void
*/
void print_liste(sommet_t * tete){
	if (!tete) printf("Liste vide\n");
	printf("Sommet numero %d\tDanger = %d\n", tete->nom, tete->danger);
	if (tete->suivant) print_liste(tete->suivant);
}

/* Place un sommet (sommet_t) dans la liste des sommets triée par dangerosité
	à partir d'un élément du tableau pi
	--> renvoie un pointeur vers la tete de la liste triée
*/
sommet_t* place_danger(int pi_elem, int pi_suiv, int num_sommet, sommet_t * tete_liste_danger, sommet_t * a_placer) {

	a_placer->danger = pi_suiv - pi_elem;
	a_placer->nom = num_sommet;

	// Pointeurs de parcours de la liste
	sommet_t * precedent = NULL;
	sommet_t * courant = tete_liste_danger;

	while ((courant) && (courant->danger > a_placer->danger))
	{
		precedent = courant;
		courant = courant->suivant;
	}

	// (Demi-)insertion de l'element a placer (courant peut etre NULL)
	a_placer->suivant = courant;

	// Si placement en tete de liste
	if(!precedent) return a_placer;

	// Sinon, rearrangement de la liste
	precedent->suivant = a_placer;
	return tete_liste_danger;

}

/* Désalloue la mémoire allouée pour une liste à partir de sa tete 
	--> void
*/
void free_liste(sommet_t * tete){

	sommet_t * temp;
	sommet_t * courant;
	courant = tete;
	
	while (courant)
	{
		temp=courant;
		courant=courant->suivant;
		free(temp);
	}

}

/* Remplit une matrice de noms de sommets par ordre de dangerosité de ceux-ci
	à partir des indices de début et de fin du tableau et de l'indice central (tronqué si taille paire)
	(FONCTION RECURSIVE)
	--> void

	AMELIORATIONS :
	- faire un tableau de correspondance entre les indices i (pour les bords)
		et l'offset correspondant par rapport au milieu
*/
void remplir_matrice(int** matrice, int ind_debut, int ind_fin, int milieu, sommet_t * plus_dangereux_non_place){
	int taille = ind_fin-ind_debut+1; // evite de calculer a chaque verification

	if (taille < 1) return; // pas de carre
	if (taille < 2) {	    // pas de coin ni de bord si taille 1x1
		printf("matrice[%d][%d]\n",ind_debut, ind_debut); // ind_debut = ind_fin
		return;
	}
	
	printf("taille=%d\n",ind_fin-ind_debut+1);
	printf("debut=%d\tfin=%d\n",ind_debut, ind_fin);

	printf("COINS\n");
	// COINS
	for (int i = 0; i < 4; i++)
	{
		printf("matrice[%d][%d]\n",i&2 ? ind_fin : ind_debut, i&1 ? ind_fin : ind_debut);
		// matrice[i&2 ? ind_fin : ind_debut][i&1 ? ind_fin : ind_debut] = plus_dangereux_non_place->nom;
		// plus_dangereux_non_place = plus_dangereux_non_place->suivant;
	}

	if (taille < 3) return; // pas de bord si taille 2x2

	// BORDS
	printf("\nBORDS :\n");
	for (int i = 0; i < ind_fin-ind_debut-1; i++)
	{ // nb "rotations" = nb elements sur un bord
		for (int j = 0; j < 4; j++)
		{ // 4 elements par rotation
			printf("matrice[%d][%d]\n",j&1 ? milieu + ((int)((i+1)/2)*(i&1 ? 1 : -1)) : (ind_fin-ind_debut)*(j>>1)+ind_debut 
									,j&1 ? (ind_fin-ind_debut)*(j>>1)+ind_debut : milieu + ((int)((i+1)/2)*(i&1 ? 1 : -1))); // j va de 0 a 3 => pas besoin de masque apres shift
			//matrice[j&1 ? (ind_fin-ind_debut)/2+ind_debut + ((int)((i+1)/2)*(i&1 ? 1 : -1)) : (ind_fin-ind_debut)*(j>>1)+ind_debut][j&1 ? (ind_fin-ind_debut)*(j>>1)+ind_debut : (ind_fin-ind_debut)/2+ind_debut + ((int)((i+1)/2)*(i&1 ? 1 : -1))] = plus_dangereux_non_place -> nom;
			//plus_dangereux_non_place = plus_dangereux_non_place->suivant;
		}
		printf("\n");
	}
	
	remplir_matrice(matrice,ind_debut+1,ind_fin-1,milieu,plus_dangereux_non_place);
}
