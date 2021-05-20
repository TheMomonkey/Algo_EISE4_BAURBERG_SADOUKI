#include <stdio.h>
#include <stdlib.h>

#ifndef ALGO_DANGER1__H
#define ALGO_DANGER1__H

#define NB_SOMMETS 9
#define NB_ARETES 10
#define N 3

typedef struct sommet_algo1_s
{
    int nom;
	int danger;
	char place; // place ou non (char car moins encombrant, seulement 0 ou 1)
    struct sommet_algo1_s * suivant;
} sommet_algo1_t;

/* Affichage d'une liste grace a sa tete 
	--> void
*/
void print_liste(sommet_algo1_t * tete);

/* Place un sommet (sommet_algo1_t) dans la liste des sommets triée par dangerosité
	à partir d'un élément du tableau pi
	--> pointeur vers la tete de la liste triée
*/
sommet_algo1_t* place_danger(int pi_elem, int pi_suiv, int num_sommet, sommet_algo1_t * tete_liste_danger, sommet_algo1_t * a_placer);

/* Désalloue la mémoire allouée pour une liste à partir de sa tete 
	--> void
*/
void free_liste(sommet_algo1_t * tete);

/* Remplit une matrice de noms de sommets par ordre de dangerosité de ceux-ci
	à partir des indices de début et de fin du tableau (FONCTION RECURSIVE)
	--> void
*/
void remplir_matrice(int** matrice, int* pi, int* alpha, int ind_debut, int ind_fin, int milieu, sommet_algo1_t * plus_dangereux_non_place);

/* Affichage de la matrice dans un fichier "nom_fichier"
	--> void
*/
void fprint_matrice(int** matrice, const char * nom_fichier);

/* Mise à jour du sommet le plus dangereux non placé
	--> void
*/
sommet_algo1_t * maj_sommet_a_placer(sommet_algo1_t * a_placer, int ** matrice, int* pi, int* alpha, sommet_algo1_t ** plus_dangereux_non_place, int i, int j);

int get_voisins(int voisins[8][2], int lig, int col);

#endif