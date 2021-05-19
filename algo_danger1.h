#include <stdio.h>
#include <stdlib.h>

#ifndef ALGO_DANGER1__H
#define ALGO_DANGER1__H

#define NB_SOMMETS 9
#define NB_ARETES 10
#define N 8

typedef struct sommet_s
{
    int nom;
	int danger;
    struct sommet_s * suivant;
} sommet_t;

/* Affichage d'une liste grace a sa tete 
	--> void
*/
void print_liste(sommet_t * tete);

/* Place un sommet (sommet_t) dans la liste des sommets triée par dangerosité
	à partir d'un élément du tableau pi
	--> pointeur vers la tete de la liste triée
*/
sommet_t* place_danger(int pi_elem, int pi_suiv, int num_sommet, sommet_t * tete_liste_danger, sommet_t * a_placer);

/* Désalloue la mémoire allouée pour une liste à partir de sa tete 
	--> void
*/
void free_liste(sommet_t * tete);

/* Remplit une matrice de noms de sommets par ordre de dangerosité de ceux-ci
	à partir des indices de début et de fin du tableau (FONCTION RECURSIVE)
	--> void
*/
void remplir_matrice(int** matrice, int ind_debut, int ind_fin, int milieu, sommet_t * plus_dangereux_non_place);

#endif