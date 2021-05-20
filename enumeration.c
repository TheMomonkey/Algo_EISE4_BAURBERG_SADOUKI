#include <stdio.h>
#include <stdlib.h>
#include "instanciation.h"
#include <math.h>

/* Définition et déclaration de la structure de données element_matrice_t et des variables globales */
int nb_sommets,voisins_tot,nb_possibilites=0,flag_permutation=0;

typedef struct element_matrice_s{
	int** tab; 
	struct element_matrice_s* suivant;
}element_matrice_t;

element_matrice_t* ensemble_matrices,meilleure_solution;

/* Création d'un élément de la liste */
void nouvel_element(element_matrice_t** nouveau, int* tab_nouveau){
	int k=0;
	/*Allocation mémoire de l'élément */
	*nouveau= (element_matrice_t*) malloc(sizeof(element_matrice_t));
	/*Allocation mémoire du tableau tab qui est à 2 dimensions */
	(*nouveau)->tab = (int**) malloc(sizeof(int*)*sqrt(nb_sommets));
	for(int i=0;i<sqrt(nb_sommets);i++)
		(*nouveau)->tab[i] = (int*) malloc(sizeof(int)*sqrt(nb_sommets));

	/* Initialisation des champs tab et suivant de element_matrice_t */
	for(int i=0;i<sqrt(nb_sommets);i++){
		for(int j=0;j<sqrt(nb_sommets);j++){
			(*nouveau)->tab[i][j]=tab_nouveau[k];
			k++;
		}
	}
	(*nouveau)->suivant=NULL;
}

/* Ajout d'un élément dans la liste représentant l'ensemble des matrices générées automatuiment */ 
void ajout_liste(int* tab_nouveau){
	element_matrice_t* nouveau;
	/* Création d'un nouvel élément de la liste */
	nouvel_element(&nouveau,tab_nouveau);
	/* Lien du reste de la liste avec le nouvel élément créé 
	 * (marche pour l'initialisation de la liste également)*/ 
	if(ensemble_matrices!=NULL){
		element_matrice_t* actuel;
		actuel=ensemble_matrices;
		while(actuel->suivant!=NULL){
			actuel=actuel->suivant;
		}
		actuel->suivant=nouveau;
	}
	else
		ensemble_matrices=nouveau;
}

void free_liste(){
	element_matrice_t* actuel=ensemble_matrices;
	while(actuel->suivant!=NULL){
		element_matrice_t* prec=NULL;
		for(int i=0;i<sqrt(nb_sommets);i++){
			free(actuel->tab[i]);
		}
		free(actuel->tab);
		prec=actuel;
		actuel=actuel->suivant;
		prec->suivant=NULL;
		free(prec);
	}
	element_matrice_t* prec=NULL;
	for(int i=0;i<sqrt(nb_sommets);i++){
		free(actuel->tab[i]);
	}
	free(actuel->tab);
	prec=actuel;
	free(prec);
}

/* Réadaptation du pseudo-code vu en TD avec M.Ouzia sur la méthode du pivot */
void nouvelle_permutation(int** tab){
	/* Définition de la case de pivot */
	int pivot=(nb_sommets-2);
	while((pivot!=-1) && *(*(tab)+pivot+1) < *(*(tab)+pivot))  
		pivot--;
	// Quand on obtient tab_final [n,n-1,....,1], pivot =-1, 
	// car le pivot est la case après la fin de la séquence croissante en partant de la droite 
	
	/* Échange entre la dernière case du tableau et la case du pivot */
	if(pivot>=0){
		int dernier=nb_sommets-1;
		while(*(*(tab)+dernier) < *(*(tab)+pivot) )
			dernier--;
		int echange;
		echange=*(*(tab)+pivot);
		*(*(tab)+pivot)=*(*(tab)+dernier);
		*(*(tab)+dernier)=echange;

	/* Inverser le tableau à partir du pivot+1 */
		pivot++;
		dernier=nb_sommets-1;
		while(pivot<dernier){
			echange=*(*(tab)+pivot);
			*(*(tab)+pivot)=*(*(tab)+dernier);
			*(*(tab)+dernier)=echange;
			pivot++;
			dernier--;
		}
	}
	else // si pivot=-1, on a fini la permutation et on obtient le tableau de la forme [n,n-1,...,1]
		flag_permutation++;	
}

/* On veut enir après générations successives de successseurs avec la méthode
 * du pivot. Le pseudo-code de cette fonction a été présenté en cours par M.Ouzia */

void enumerer(){
	/* Création du tableau [1,.....,nb_sommets] et remplissage */
	int *tab;
	tab= (int*) malloc(sizeof(int)*nb_sommets);
	for(int i=0;i<nb_sommets;i++){
		tab[i]=i+1;
	}

	/* On ajoute l'énumération à la liste pointée par ensemble_matrices */
	ajout_liste(tab);

	/* Utilisation de la fonction de permutation pour générer une autre énumération */
	 while(!flag_permutation){
		nouvelle_permutation(&tab);
		ajout_liste(tab);
	}
	/* Désallouement de la mémoire */
 	free(tab);	 
}
 
/* Fonction de vérification à impllémenter pour vérifier la dangerosité des configurationz de fûts
 * Pas encore implémentée car il y a des problématiques sur la générations des énumérations pour une grille de plus de 4 sommets */
//int verification(){}

/* Programme principalpour tester l'énumération à partir de la génération automique d'instances en entrée  */
int main(int argc, char** argv){
	int *pi,*alpha;
	ensemble_matrices=NULL;
	if(argc<2){
		printf("Veuillez saisir le nombre de sommets, apres le nom du programme executable, avec lesquels vous voulez creer la prochaine instance. \nSi vous voulez gagner du temps ou diminuer la complexite de l\'instanciation, vous pouvez renseigner un deuxième entier qui donnera la limite de voisins dangereux pour chaque sommet. \n");
	       exit(1);
	}

	/* Récupération d'un graphe instancié automatiquement via les tableaux pi et alpha */
	voisins_tot=instanciation(argc,argv,&pi,&alpha);
	nb_sommets=atoi(argv[1]);
	
	/* Vérification que l'on a bien récupéré pi et alpha */
	printf("Tableau Pi = [");
	for(int i=0;i<nb_sommets;i++){
		if(!i)
			printf("%d",pi[i]);
		else
			printf(", %d", pi[i]);	
	}
	printf("]\nTableau Alpha = [");
	for(int i=0;i<voisins_tot;i++){
		if(!i)
			printf("%d",alpha[i]);
		else
			printf(", %d", alpha[i]);
	}
	printf("]\n");

	/*Commencement de l'énumération */
	enumerer();
	
	/* On parcourt toute la liste pointée par ensemble_matrices.et on affiche les énumérations*/
	element_matrice_t* actuel=ensemble_matrices;
	while(actuel->suivant!=NULL){
		printf("Matrice=\n[");
		for(int i=0;i<sqrt(nb_sommets);i++){
			for(int j=0;j<sqrt(nb_sommets);j++){
				if(!j)
					printf("%d",actuel->tab[i][j]);
				else
					printf(", %d",actuel->tab[i][j]);
			}
			printf("]\n[");
		}
		actuel=actuel->suivant;
	}
	// Désallocation de la mémoire
	free_liste();
	free(pi);
	free(alpha);
	return 0;
	
}
