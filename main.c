//Réseau de neuronne Self Organizing Map
//Ce code est fait pour fonctionner sous Linux
//Remplacer le parser pour multi-plateforme
//main.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "data.h"
#include "formulas.h"
#include "neural_map.h"

/*TODO(1): -Adapter alpha(voir avec prof)
*/

int main(int argc, char *argv[])
{
    FILE *inp_line = fopen(argv[1], "r");
    FILE *inp_data = fopen(argv[1], "r");
    FILE * input = fopen(argv[1], "r");
    srand((unsigned)getpid());
    
    //Initialisation de la base de donnée
    data_info* info = malloc(sizeof(data_info));
    info->nb_line = countLine(inp_line);
    info->nb_data = countData(inp_data);
	data_v** iris_data = malloc(info->nb_line * sizeof(data_v));
	double* _vec_moyen = malloc(sizeof(double)*info->nb_data);
    initData(iris_data, info, input);
    normalisation(iris_data, info);
    printBDD(iris_data, info->nb_line);
    fclose(inp_line);
    fclose(inp_data);
    fclose(input);
    
    //Normalisation des données
    
    //Initialisation du vecteur moyen
	_vec_moyen = average(iris_data, info->nb_line);
	printf("\nvecteur_moyen: ");
	printVec(_vec_moyen, info->nb_data);

	//Initialisation de la grille
	info->nb_neurone = 5*sqrt(info->nb_line);
	grille grid;
	grid = initGrid(info->nb_neurone);
	info->nb_neurone = grid.nb_line*grid.nb_column;
	
    printf("\nnb_neurone ajusté apres analyse topologique: %d\n", info->nb_neurone);      
    
    //Initialisation reseau neurones  
    neural_m** neural_map = malloc(info->nb_neurone * sizeof(neural_m*));
	initNeuralMap(neural_map, _vec_moyen, info->nb_neurone);
	//printVecNeuralMap(neural_map, nb_neurone);
	
    
	//Initialisation de l'apprentissage
	printf("\nDEBUT APPRENTISSAGE");
    BMU* bmu_list;
    int * indice_voisins;
    int rayon = ceil(sqrt(info->nb_neurone/2)/2);
    int i_ite = 0;
    double alpha;
    double rapport;
    int nb_iteration = 500* info->nb_line;
    int phase2 = nb_iteration/4;
	while(i_ite < nb_iteration)
	{ 
	    grid.nb_voisin[0] = (int) pow(rayon*2+1, 2);
	    if(i_ite == 0)
	    	alpha = 0.8f;
	    else if(i_ite == phase2/3 && rayon > 1)
	    	rayon--;
	    else if(i_ite == phase2/2 && rayon > 1)
	    	rayon--;
	    else if(i_ite == phase2)
	    {
	    	rayon = 2;
	    	alpha = 0.08f;
	    }
	   	else
	   	{
	    	rapport = (double) i_ite/nb_iteration;
	    	alpha = alpha * (1.0f- rapport);
	    } 
	    int* tmp_ind = shuffle2(info->nb_line);
	    for(int i = 0; i < info->nb_line; i++)
	    {
			//MEILLEUR NEURONES
			bmu_list = getBMU(neural_map, iris_data[tmp_ind[i]], info->nb_neurone);
			//printf("\nnombre de bmu : %d\n", bmu_list->nb_bmu);
		    //printf("\nNombre de voisins approximatif = %zu\n", grid.nb_voisin[0]);
		    //printf("\n\nici rayon %d\n\n", rayon);
			indice_voisins = voisinage(rayon, bmu_list->best_indice[rand()%bmu_list->nb_bmu], grid);
			resetGrid(grid._rect, grid.nb_line, grid.nb_column);
			apprendre(neural_map, iris_data[tmp_ind[i]], alpha, indice_voisins, grid.nb_voisin[0]);
		    //printVecNeuralMap(neural_map, nb_neurone);
		    	
		    //free(llist);
        }
        printf("\nProgression : %f%% alpha: %lf", rapport*100 ,alpha);
        i_ite++;
	} 
	printVecNeuralMap(neural_map, info->nb_neurone);
	return 0;	
}
