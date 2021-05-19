#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_sommets,nb_voisins_max,cpt=0;

// Définition de la structure représentant le sommet
typedef struct sommet_s{
	int num_sommet; /* Numéro du sommet */
	int nb_voisins; /* Nombre de voisins que le sommet est censé avoir */
	struct sommet_s* tab_voisins; /* Tableau répertoriant tous les voisins */
	int alloue; /* Indique si de la mémoire a été allouée pour tab_voisins (n'est pas alloué si 0 voisins */
	int place; /* Nombre de voisins dont le sommet dispose au moment où on regarde */
}sommet_t;

// Initialisation des champs de la structure représentant les sommets
void init_sommets(sommet_t* ensemble_sommets){
	for(int i=0;i<nb_sommets;i++){
		/* On veut avoir des sommets numérotés à partir de 1 */
		ensemble_sommets[i].num_sommet=i+1;

		/* Mise à jour du seed de la fonction rand pour avoir des valeurs random qui diffèrent */
		srand(cpt);
		ensemble_sommets[i].nb_voisins= rand() % (nb_voisins_max) ;
		if(ensemble_sommets[i].nb_voisins!=0){
			ensemble_sommets[i].tab_voisins = (sommet_t*) malloc(sizeof(sommet_t)*ensemble_sommets[i].nb_voisins);
			ensemble_sommets[i].alloue=1;
		}
		else
			ensemble_sommets[i].alloue=0;
		ensemble_sommets[i].place = 0;
		cpt++;
		printf("le sommet %d a %d voisins\n",i+1,ensemble_sommets[i].nb_voisins);
	}
}

void free_sommets(sommet_t* ensemble_sommets){
	for(int i=0;i<nb_sommets;i++){
		if(ensemble_sommets[i].alloue)
			free(ensemble_sommets[i].tab_voisins);
	}
	free(ensemble_sommets);
}

int main(int argc, char** argv){
	int flag=0,flag_tab=0,nouveau_voisin,voisins_tot=0;

	/* Récupération du nombre de sommets à inclure dans le graphe */
	nb_sommets=atoi(argv[1]);

	/* Récupération du nombre de voisins maximum par sommet à inclure dans le graphe pour notamment réduire le temps d'instanciation */
	if(argc==3)
		nb_voisins_max=atoi(argv[2]);
	else
		nb_voisins_max=nb_sommets;
	
	printf("nb_sommets= %d nb_voisins_max=%d \n",nb_sommets,nb_voisins_max);

	int* pi=(int*) malloc(sizeof(int)*nb_sommets);
	/* Initialisation des structures nécessaires */
	sommet_t* ensemble_sommets = (sommet_t*) malloc(sizeof(sommet_t)*nb_sommets);
	init_sommets(ensemble_sommets);
	printf("fin init_sommets\n");

	/* Répartition des voisins à chacun des sommets du graphe */
	for(int i=0;i<nb_sommets;i++){
		/* Remplissage au fur et à mesure du tableau pi pour indexer les voisins de chaque voisin */
		if(!i)
			pi[i]=0;
		else
			pi[i]=pi[i-1] + ensemble_sommets[i-1].nb_voisins;

		printf("le sommet %d a un nb_voisins de %d\n",i+1,ensemble_sommets[i].nb_voisins);
		
		while(ensemble_sommets[i].place<ensemble_sommets[i].nb_voisins){
			printf("num_placés= %d \n",ensemble_sommets[i].place);
			/* Création de tab_remplissage pour éviter de tomber dans le cas où une valeur de nb_voisins pour un des sommets n'est pas en accord avec le nombre de
			 * voisins que peuvent prendrele reste des sommets dans le graphe */
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
							//printf("Toujours pas le num %d \n",m);
							flag=0;
							flag_tab=0;
							break;
						}
				
					}
				}
			}
			/* Si nous étions tombés dans une boucle infinie alors le champ nb_voisins attribué au sommet n'est pas en accord avec le
			 * nombre de voisins que peut vraiment avoir ce sommet */
			if(flag_tab){
				printf("tab_flag");
				flag_tab=0;
				ensemble_sommets[i].nb_voisins=ensemble_sommets[i].place;
			}

			/* Sinon on affecte le sommet choisi comme nouveau voisin di sommet étudié */
			else{
				ensemble_sommets[i].tab_voisins[ensemble_sommets[i].place]=ensemble_sommets[nouveau_voisin];
				ensemble_sommets[i].place++;
				ensemble_sommets[nouveau_voisin].tab_voisins[ensemble_sommets[nouveau_voisin].place]=ensemble_sommets[i];
				ensemble_sommets[nouveau_voisin].place++;
				printf("Un voisin en plus!\n");
			}
			flag=0;
			voisins_tot+=ensemble_sommets[i].nb_voisins;
			
		}
		for(int k=0;k<ensemble_sommets[i].nb_voisins;k++){
			printf("Voisin n°%d = %d \n",k,ensemble_sommets[i].tab_voisins[k].num_sommet);
		}
	}

	/* Affichage du tableau pi */
	printf("Tableau pi = [%d",pi[0]);
	for(int i=1; i<nb_sommets;i++){
		printf(", %d", pi[i]);
	}
	printf("] \n");

	/* Remplissage et affichage du tableau alpha (ne pouvait être fait en même temps que la répartition des voisins car le nombre de voisins peut être diminué si
	 * besoin lors de la répartition */
	int* alpha = (int*) malloc(sizeof(int)*voisins_tot);
	int indice_alpha=0;
	printf("Tableau alpha = [");
	for(int i=0;i<nb_sommets;i++){
		for(int k=0;k<ensemble_sommets[i].nb_voisins;k++){		
			alpha[indice_alpha]=ensemble_sommets[i].tab_voisins[k].num_sommet;
			if(!indice_alpha)
				printf("%d",alpha[indice_alpha]);
			else
				printf(", %d", alpha[indice_alpha]);
			indice_alpha++;
		}
	}
	printf("] \n");
	free_sommets(ensemble_sommets);
	free(pi); // A enlever si on doit retourner les tableaux en entrée de l'algo principal
	free(alpha); 
	return 0;
}



