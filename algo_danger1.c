#include <stdio.h>
#include <stdlib.h>
#include "algo_danger1.h"

int dangerosite = 0;

/* Affichage d'une liste grace a sa tete 
	--> void
*/
void print_liste(sommet_algo1_t * tete){
	if (!tete) printf("Liste vide\n");
	printf("Sommet numero %d\tDanger = %d\n", tete->nom, tete->danger);
	if (tete->suivant) print_liste(tete->suivant);
}



/* Place un sommet (sommet_algo1_t) dans la liste des sommets triée par dangerosité
	à partir d'un élément du tableau pi
	--> renvoie un pointeur vers la tete de la liste triée
*/
sommet_algo1_t* place_danger(int pi_elem, int pi_suiv, int num_sommet, sommet_algo1_t * tete_liste_danger, sommet_algo1_t * a_placer) {

	a_placer->danger = pi_suiv - pi_elem;
	a_placer->nom = num_sommet;
	a_placer->place = 0;

	// Pointeurs de parcours de la liste
	sommet_algo1_t * precedent = NULL;
	sommet_algo1_t * courant = tete_liste_danger;

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
void free_liste(sommet_algo1_t * tete){

	sommet_algo1_t * temp;
	sommet_algo1_t * courant;
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
void remplir_matrice(int** matrice, int* pi, int* alpha, int ind_debut, int ind_fin, int milieu, sommet_algo1_t * plus_dangereux_non_place){
	int taille = ind_fin - ind_debut+1; // evite de calculer a chaque verification
	sommet_algo1_t * a_placer = (sommet_algo1_t*)(malloc(sizeof(sommet_algo1_t)));

	if (taille < 1) return; // pas de carre
	if (taille < 2) {	    // pas de coin ni de bord si taille 1x1
		printf("matrice[%d][%d]\n",ind_debut, ind_debut); // ind_debut = ind_fin
		a_placer = maj_sommet_a_placer(a_placer,matrice,pi,alpha,&plus_dangereux_non_place,ind_debut,ind_debut);
		matrice[ind_debut][ind_debut] = a_placer->nom; // c'est le dernier
		return;
	}
	
	printf("taille=%d\n",ind_fin-ind_debut+1);
	printf("debut=%d\tfin=%d\n",ind_debut, ind_fin);


	printf("COINS\n");
	// COINS
	for (int i = 0; i < 4; i++)
	{
		printf("matrice[%d][%d]\n",i&2 ? ind_fin : ind_debut, i&1 ? ind_fin : ind_debut);
		matrice[i&2 ? ind_fin : ind_debut][i&1 ? ind_fin : ind_debut] = plus_dangereux_non_place->nom;
		plus_dangereux_non_place = plus_dangereux_non_place->suivant;
	}

	if (taille < 3) return; // pas de bord si taille 2x2

	// BORDS
	int offset = 0, lig = 0, col = 0;
	printf("\nBORDS :\n");
	for (int i = 0; i < ind_fin-ind_debut-1; i++)
	{ // nb "rotations" = nb elements sur un bord
		offset = ((int)((i+1)/2)*(i&1 ? 1 : -1));
		for (int j = 0; j < 4; j++)
		{ // 4 elements par rotation
			//printf("matrice[%d][%d]\n",j&1 ? milieu + offset : (ind_fin-ind_debut)*(j>>1)+ind_debut 
									//,j&1 ? (ind_fin-ind_debut)*(j>>1)+ind_debut : milieu + offset); // j va de 0 a 3 => pas besoin de masque apres shift
			lig = j&1 ? milieu + offset : (ind_fin-ind_debut)*(j>>1)+ind_debut;
			col = j&1 ? (ind_fin-ind_debut)*(j>>1)+ind_debut : milieu + offset;
			a_placer = maj_sommet_a_placer(a_placer,matrice,pi,alpha,&plus_dangereux_non_place,lig,col);
			printf("DEHORS FCT plus_dangereux_non_place->nom = %d\n",plus_dangereux_non_place->nom);
			matrice[lig][col] = a_placer->nom;
			printf("PLACE matrice[%d][%d] = %d\n",lig,col,matrice[lig][col]);
		}
		printf("\n");
	}
	
	free(a_placer);

	remplir_matrice(matrice,pi,alpha,ind_debut+1,ind_fin-1,milieu,plus_dangereux_non_place);
}

/* Remplit le tableau des voisins du sommet de coordonnees (lig,col)
	--> renvoie le nb d'elements remplis dans le tableau

	AMELIORATIONS :
	- trouver un schema similaire a celui de remplir_matrice pour recuperer les indices des voisins
*/
int get_voisins(int voisins[8][2], int lig, int col){

	//printf("\n\n/*******VOISINS*******/\n");
	
	int nb_carres = N/2; // nb de carres imbriques dans le carre (commence a 0)

	// Determination du carre actuel (0 = carre exterieur, juste coins+bords)
	int num_carre_max_lig = lig <= (nb_carres) ? lig : -lig+2*nb_carres; // fonction triangle
	int num_carre_max_col = col <= (nb_carres) ? col : -col+2*nb_carres; // fonction triangle
	int carre_actuel = num_carre_max_lig < num_carre_max_col ? num_carre_max_lig : num_carre_max_col;
	
	int nb_voisins_a_verifier = 0;

	// bornes (communes aux lignes+colonnes car carre)
	int indice_max = N-1 - carre_actuel;
	int indice_min = carre_actuel;

	for (int i = -1; i < 2; i++) {
		int l = lig + i;
		if (l < 0 || l > N-1) continue;

		for (int j = -1; j < 2; j++) {
			int c = col + j;
			if (c < 0 || c > N-1) continue;
			if ((l <= indice_min || l >= indice_max || c <= indice_min || c >= indice_max) && (c != col || l != lig)){
				voisins[nb_voisins_a_verifier][0] = l;
				voisins[nb_voisins_a_verifier][1] = c;
				printf("l=%d\n",l);
				printf("c=%d\n\n",c);		
				nb_voisins_a_verifier++;
			}
		}
	}

	printf("NB_VOISINS = %d\n", nb_voisins_a_verifier);

	return nb_voisins_a_verifier;
}

/* AAAAAAAAAAAAAAAAAAAAA FINIIIIIIIIIIIIIIIIIR */
sommet_algo1_t * maj_sommet_a_placer(sommet_algo1_t * a_placer, int** matrice, int* pi, int* alpha, sommet_algo1_t ** plus_dangereux_non_place, int lig, int col){

	printf("plus_dangereux_non_place->nom = %d\n",(*plus_dangereux_non_place)->nom);
	a_placer = *plus_dangereux_non_place;
	printf("i=%d,j=%d\n",lig,col);

	if ((lig==0 && (col==0 || col==N-1)) || (lig==N-1 && (col==0 || col==N-1))) { // si sur coin, placement immediat
		*plus_dangereux_non_place = (*plus_dangereux_non_place)->suivant;
		return a_placer;
	}

	int voisins[8][2] = {0}; // tableau des coordonnees des voisins
	int nb_voisins = get_voisins(voisins, lig, col); // remplit "voisins" et renvoie nb cases remplies


	for (int i = 0; i < nb_voisins; i++)
	{
		// un_voisin = un voisin dans la matrice dont l'indice est compatible avec le tableau pi (a 1 pres)
			// (les indices de pi commencent a 0 et les noms de sommets a 1)
		int un_voisin = matrice[voisins[i][0]][voisins[i][1]];
		printf("un_voisin = %d, i = %d\n",un_voisin, i);

		if (un_voisin)
		{ // car il peut ne pas etre encore place
			
			for (int j = 0; j < a_placer->danger; j++)
			{
				if (alpha[pi[a_placer->nom - 1] + j] == un_voisin)
				{
					if (a_placer->suivant) {
						a_placer = a_placer->suivant;
						while (a_placer->place == 1) {
							if (a_placer->suivant) a_placer = a_placer->suivant;
							else a_placer = plus_dangereux_non_place;
						}
						printf("weshwesh passe a %d\n", a_placer->nom);
						i = -1; // changement de sommet => re-verification
						break;
					}
					else {
						a_placer = *plus_dangereux_non_place; // si on arrive a la fin de la liste (tous provoquant une liaison dangereuse), autant placer le plus dangereux
						dangerosite++;
						break;
					}
				}
			}
		}
	}

	a_placer->place = 1;
	printf("a_placer->nom = %d\n",a_placer->nom);
	if ((a_placer->nom == (*plus_dangereux_non_place)->nom) && ((*plus_dangereux_non_place)->suivant) != NULL)) *plus_dangereux_non_place = (*plus_dangereux_non_place)->suivant;

	return a_placer;

}

/* Affichage de la matrice dans un fichier "nom_fichier"
	--> void
*/
void fprint_matrice(int** matrice, const char * nom_fichier){
	FILE * fd = fopen(nom_fichier, "w");

	fprintf(fd, "     ");
	for (int j = 0; j < N; j++)
	{
		fprintf(fd, "%d     ", j);
	}
	fprintf(fd, "\n  ");
	for (int j = 0; j < N; j++)
	{
		fprintf(fd, "______");
	}
	fprintf(fd, "_\n");

	for (int i = 0; i < N; i++)
	{
		fprintf(fd, "%d |", i);
		for (int j = 0; j < N; j++)
		{
			fprintf(fd, "_%03d_|", matrice[i][j]);
		}
		fprintf(fd, "\n");
	}
	

	fclose(fd);
}
