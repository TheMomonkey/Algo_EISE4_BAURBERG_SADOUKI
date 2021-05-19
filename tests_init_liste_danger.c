#include <stdio.h>
#include <stdlib.h>

#define NB_SOMMETS 9
#define NB_ARETES 10

typedef struct sommet_s
{
    int nom;
	int danger;
    struct sommet_s * suivant;
} sommet_t;


/* Affichage d'une liste grace a sa tete */
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


int main(int argc, char const *argv[])
{
	
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

	free_liste(tete_liste_danger);
    return 0;
}
