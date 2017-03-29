#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "data.h"
#include "formulas.h"
#include "neural_map.h"


//Initialise le nbre de ligne/colonne
grille initGrid(int nb_neurone)
{   
    int nb_column = 10;
    int nb_ligne;
    double tmp;
    
    while(nb_neurone - nb_column >0) 
        nb_column *= 10;    
    nb_column /= 10;    
    //printf("\n base:%d\n", nb_column);
    if(nb_column > 10)
        nb_column = nb_column/10;
    else if(nb_column > 100)
        nb_column = nb_column/100;
    else if(nb_column > 1000)
        nb_column = nb_column/1000;
    else if(nb_column > 10000)
        nb_column = nb_column/10000;    
    tmp = (double)nb_neurone/nb_column;
    nb_ligne = (int) ceil(tmp);
    
    grille test;
    test.nb_voisin = malloc(sizeof(size_t));
    test._rect = (int **)malloc(sizeof(int*)*nb_ligne);
    test.nb_line = nb_ligne;
    test.nb_column = nb_column;
    int y, i, z;
    
    for(i = 0, z = 0; i < test.nb_line; i++)
    {
        test._rect[i] = (int*)malloc(sizeof(int)*nb_column);
        for(y = 0; y < test.nb_column; y++)
        {
            test._rect[i][y] = (int) z++;
        }
    }
    return test;
}

void resetGrid(int** grid, int nb_ligne, int nb_colonne)
{
    int y, i, z;
    for(i = 0, z = 0; i < nb_ligne; i++)
    { 
        for(y = 0; y < nb_colonne; y++)
        {
           grid[i][y] = z;
           z++;
        }
    }
}

void printGrid(int** grid, int nb_ligne, int nb_colonne)
{
    int y, i;
    for(i = 0; i < nb_ligne; i++)
    {
        if (i == 1)
            printf("  ");
        printf("\n");
        for(y = 0; y < nb_colonne; y++)
        {
            printf("%d ", grid[i][y]);
        }
    }
}

void initNeuralMap(neural_m** neural_map, double* vec_moyen, int nb_neurones)
{
    for(int i = 0; i < nb_neurones; i++)
    {
        neural_map[i] = malloc(sizeof(neural_m*));
        neural_map[i]->mem = malloc(sizeof(double) * 4);
        for(int y = 0; y < 4; y++)
        {
            neural_map[i]->mem[y] = rand_double(vec_moyen[y], 0.02, 0.05);
        }
    }
}



void printVecNeuralMap(neural_m** neural_map, int nb_neurones)
{
    printf("\nDONNEES DES NEURONES:\n");
   
    for(int i = 0; i < nb_neurones; i++)
    {   
        printf("n°%d| ", i+1);
        for(int y = 0; y < 4; y++)
        {
            printf("%lf  ", neural_map[i]->mem[y]);
        }
        printf("\n");
    }
}

void printIntVec(int* indice_voisins, size_t size)
{
    printf("\n");
    for(int i = 0;i < size; i++)
    {
        if(i%5 == 0)
            printf("\n");
        printf("%d ", indice_voisins[i]);
    }
}

//Retourne la liste des voisins du BMU choisi
int* voisinage(int rayon, int bmu, grille neural_grid)
{
    int i_bmu, y_bmu;
    for(int i = 0; i < neural_grid.nb_line; i++)
    {
        for(int y = 0; y < neural_grid.nb_column; y++, bmu--)
            if(bmu == 0)
            {
                //neural_grid._rect[i][y] = -5;
                i_bmu = i;
                y_bmu = y;
            }  
    }                         
    int c = 0;
    int d = 0;
    if (rayon%2 == 0)
        rayon+=1;
    //printf("\nici2 rayon:%d nb_voisin_max:%lf \n", rayon, pow(rayon*2+1, 2));
    int *_v = malloc(sizeof(int)*pow(rayon*2+1,2));
    int vi = 0;
    //printf("\nrayon: %d bmu_i : %d bmu_y : %d", rayon, i_bmu, y_bmu);
    while(c != rayon+1)
    {
        while(d != rayon+1)
        {        
            if(i_bmu+c < neural_grid.nb_line && y_bmu+d < neural_grid.nb_column && neural_grid._rect[i_bmu+c][y_bmu+d] != -4)
            {
                _v[vi] = neural_grid._rect[i_bmu+c][y_bmu+d];
                neural_grid._rect[i_bmu+c][y_bmu+d] = -4;
                vi++;
            }
            if(i_bmu-c >= 0 && y_bmu-d >= 0 && neural_grid._rect[i_bmu-c][y_bmu-d] != -4)
            {   
                _v[vi] = neural_grid._rect[i_bmu-c][y_bmu-d];
                 neural_grid._rect[i_bmu-c][y_bmu-d] = -4;
                vi++;
            }    
            if(i_bmu+c < neural_grid.nb_line && y_bmu-d >= 0 && neural_grid._rect[i_bmu+c][y_bmu-d] != -4)
            {
                _v[vi] = neural_grid._rect[i_bmu+c][y_bmu-d];
                neural_grid._rect[i_bmu+c][y_bmu-d] = -4;
                vi++;
            }
            if(i_bmu-c >= 0 && y_bmu+d < neural_grid.nb_column && neural_grid._rect[i_bmu-c][y_bmu+d] != -4)
            {
                _v[vi] = neural_grid._rect[i_bmu-c][y_bmu+d];
                neural_grid._rect[i_bmu-c][y_bmu+d] = -4;
                vi++;  
            } 
            d++;    
        }
        d = 0; 
        c++;
    }
    neural_grid.nb_voisin[0] = vi;
    return _v;
          
}


//DEBUT APPRENTISSAGE
void apprendre(neural_m** neural_map, data_v* data, double alpha, int* voisins, size_t nb_voisin)
{
	int i = 0, y = 0;
	while(i < nb_voisin)
	{
	    y = 0;
	    while(y < 4)
	    {
	        neural_map[voisins[i]]->mem[y] += alpha * (data->vec[y] - neural_map[voisins[i]]->mem[y]);//ICI revoir la formule
	        y++;
	    }
	    i++;
	}
}

//Retourne les meilleurs neurones
BMU* getBMU(neural_m** neural_map, data_v* one_data, int nb_neurones)
{
    double min = RAND_MAX;
  
    //ici on fais la distance eulidienne et calcul de la + petite distance
    for(int i=0; i < nb_neurones; i++)
    {
        neural_map[i]->act = euclidean_distance(neural_map[i]->mem, one_data->vec);
        //printf("%d: %lf\n", i, neural_map[i]->act);
        if(neural_map[i]->act < min)
            min = neural_map[i]->act;
    }
   
 	BMU* bmu = malloc(sizeof(BMU));
 	bmu->best_indice = malloc(sizeof(int)*nb_neurones);
 	bmu->nb_bmu = 0;
    //printf("\nmeilleur_distance: %lf", min);
    int y= 0;
    //Attribution DES meilleurs neurones
    for(int i=0; i < nb_neurones; i++)
    {
        if(neural_map[i]->act == min)
        {   
            bmu->best_indice[y] = i;
            //printf("\nnouveau membre de la liste : %d\n", bmu->best_indice[y]);
            y++;
            bmu->nb_bmu++;
        }
    }
    return bmu;
}

void readll(BMU* bmu_list)
{
    for(int i = 0;i < bmu_list->nb_bmu; i++)
    {
    	printf("\nBMU n°%d : %d\n", i, bmu_list->best_indice[i]);
    }
}



