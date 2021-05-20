#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "instanciation.h"

int nb_sommets,nb_voisins_max,cpt=0,voisins_tot=0;

// Définition de la structure représentant le sommet
typedef struct sommet_s{
	int num_sommet; /* Numéro du sommet */
	int nb_voisins; /* Nombre de voisins que le sommet est censé avoir */
	struct sommet_s* tab_voisins; /* Tableau répertoriant tous les voisins */
	int alloue; /* Indique si de la mémoire a été allouée pour tab_voisins (n'est pas alloué s'il a 0 voisins) */
	int place; /* Nombre de voisins dont le sommet dispose au moment où on le regarde */
}sommet_t;

// Initialisation des champs de la structure représentant les sommets
void init_sommets(sommet_t* ensemble_sommets){
	for(int i=0;i<nb_sommets;i++){
		/* On veut avoir des sommets numérotés à partir de 1 */
		ensemble_sommets[i].num_sommet=i+1;

		/* Mise à jour du seed de la fonction rand pour avoir des valeurs random qui diffèrent */
		srand(cpt);
		ensemble_sommets[i].nb_voisins= rand() % (nb_voisins_max) ;

		/* Allocation mémoire pour le tableau de voisins et initialisation des champs restants */
		if(ensemble_sommets[i].nb_voisins!=0){
			ensemble_sommets[i].tab_voisins = (sommet_t*) malloc(sizeof(sommet_t)*ensemble_sommets[i].nb_voisins);
			ensemble_sommets[i].alloue=1;
		}
		else
			ensemble_sommets[i].alloue=0;
		ensemble_sommets[i].place = 0;
		/* A chaque incrémentation, donne un nouveau seed pour générer un entier de façon random, 
		 * Nous avons remarqué qu'utiliser time(NULL) dans srand pour cette partie ne permettait pas vraimer de diversifier
		 * les nombres de voisins */
		cpt++;
	}
}

// Désallocation de la mémoire allouée pour éviter les fuites mémoires 
void free_sommets(sommet_t* ensemble_sommets){
	for(int i=0;i<nb_sommets;i++){
		if(ensemble_sommets[i].alloue)
			free(ensemble_sommets[i].tab_voisins);
	}
	free(ensemble_sommets);
}

// Fonction d'instanciation automatique
int instanciation(int argc, const char** argv,int** p_pi,int** p_alpha){
	int flag=0,flag_tab=0,nouveau_voisin;

	/* Récupération du nombre de sommets à inclure dans le graphe */
	nb_sommets=atoi(argv[1]);

	/* Récupération du nombre de voisins maximum par sommet à inclure dans le graphe pour notamment réduire le temps d'instanciation */
	if(argc==4)
		nb_voisins_max=atoi(argv[2]);
	else
		nb_voisins_max=nb_sommets;

	/* Initialisation des structures et allocation de la mémoire nécessaire */
	*p_pi=(int*) malloc(sizeof(int)*nb_sommets);
	sommet_t* ensemble_sommets = (sommet_t*) malloc(sizeof(sommet_t)*nb_sommets);
	init_sommets(ensemble_sommets);

	/* Répartition des voisins à chacun des sommets du graphe */
	for(int i=0;i<nb_sommets;i++){
		/* Remplissage au fur et à mesure du tableau pi pour indexer les voisins de chaque sommet */
		if(!i)
			*(*(p_pi)+i)=0;
		else
			*(*(p_pi)+i)=*(*(p_pi)+i-1) + ensemble_sommets[i-1].nb_voisins;

		printf("Le sommet %d doit avoir %d voisins.\n",i+1,ensemble_sommets[i].nb_voisins);
		
		while(ensemble_sommets[i].place<ensemble_sommets[i].nb_voisins){
			/* Création de tab_remplissage pour éviter de tomber dans le cas où une valeur de nb_voisins pour un des sommets n'est pas en accord avec le nombre de
			 * voisins que peuvent prendre le reste des sommets dans le graphe */
			int tab_remplissage[nb_sommets];
			for(int j=0;j<nb_sommets;j++){
				tab_remplissage[j]=0;
			}

			/* Recherche d'un nouveau voisin possible */
			while(!flag){
				srand(time(NULL));
				nouveau_voisin = rand() % nb_sommets;
				if((i != nouveau_voisin) && (ensemble_sommets[nouveau_voisin].place<ensemble_sommets[nouveau_voisin].nb_voisins)){
					flag++;
					for(int k=0;k<ensemble_sommets[i].place;k++){
						if(ensemble_sommets[i].tab_voisins[k].num_sommet==ensemble_sommets[nouveau_voisin].num_sommet){
							flag=0;
						}
					}
				}
				/* Vérifications que nous ne sommes pas dans une boucle infinie */
				if(!flag){
					tab_remplissage[nouveau_voisin]++;
					flag=1;
					flag_tab=1;
					for(int m=0;m<nb_sommets;m++){
						if(tab_remplissage[m]==0){
							flag=0;
							flag_tab=0;
							break;
						}
				
					}
				}
			}
			/* Si sommes tombés dans une boucle infinie alors le champ nb_voisins attribué au sommet n'est pas en accord avec le
			 * nombre de voisins que peut vraiment avoir ce sommet */
			if(flag_tab){
				flag_tab=0;
				ensemble_sommets[i].nb_voisins=ensemble_sommets[i].place;
				printf("Reduction du nombre de voisins pour le sommet %d a %d voisins\n",i+1,ensemble_sommets[i].nb_voisins);
			}

			/* Sinon on affecte le sommet choisi comme nouveau voisin du sommet étudié */
			else{
				ensemble_sommets[i].tab_voisins[ensemble_sommets[i].place]=ensemble_sommets[nouveau_voisin];
				ensemble_sommets[i].place++;
				ensemble_sommets[nouveau_voisin].tab_voisins[ensemble_sommets[nouveau_voisin].place]=ensemble_sommets[i];
				ensemble_sommets[nouveau_voisin].place++;
			}
			flag=0;
			
			
		}
		/* Affichage des voisinss retenus pour le sommet */
		for(int k=0;k<ensemble_sommets[i].nb_voisins;k++){
			printf("Voisin n°%d = %d \n",k,ensemble_sommets[i].tab_voisins[k].num_sommet);
		}
		voisins_tot+=ensemble_sommets[i].nb_voisins;
	}

	/* Affichage du tableau pi */
	printf("Tableau pi = [%d",**(p_pi));
	for(int i=1; i<nb_sommets;i++){
		printf(", %d", *(*(p_pi)+i));
	}
	printf("] \n");

	/* Remplissage et affichage du tableau alpha (ne pouvait être fait en même temps que la répartition des voisins car le nombre de voisins peut être diminué si
	 * besoin lors de la répartition */	
	*p_alpha = (int*) malloc(sizeof(int)*voisins_tot);
	int indice_alpha=0;
	printf("Tableau alpha = [");
	for(int i=0;i<nb_sommets;i++){
		for(int k=0;k<ensemble_sommets[i].nb_voisins;k++){		
			*(*(p_alpha)+indice_alpha)=ensemble_sommets[i].tab_voisins[k].num_sommet;
			if(!indice_alpha)
				printf("%d",*(*(p_alpha)+indice_alpha));
			else
				printf(", %d",*(*(p_alpha)+indice_alpha));
			indice_alpha++;
		}
	}
	printf("] \n");

	/* Désalloument de la mémoire allouée */
	free_sommets(ensemble_sommets);
	return voisins_tot;
}



