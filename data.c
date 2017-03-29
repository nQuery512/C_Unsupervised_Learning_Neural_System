//Réseau de neuronne Self Organizing Map
//Ce code est fait pour fonctionner sous Linux
//data.c

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

#include "data.h"
#include "formulas.h"
#include "neural_map.h"

//Mélanger les données
void shuffle(data_v** _array, int nb_line)
{
    int y;
    srand(time(NULL));
    for(int i = 0; i < nb_line; i++)
    {
        while((y = rand()%nb_line) == i);
        char* tmp_nom = malloc(strlen(_array[y]->nom));
        double* tmp_vec = malloc(sizeof(_array[y]->vec));
    
        strcpy(tmp_nom, _array[y]->nom);
        tmp_vec = _array[y]->vec;
        double tmp_norme = _array[y]->norme;
        
        _array[y]->vec = _array[i]->vec;
        _array[i]->vec = tmp_vec;
        
        //sinon permutter le nom est inutile
        if(strcmp(_array[y]->nom, _array[i]->nom) != 0)
        {
            _array[y]->nom = _array[i]->nom;
            _array[i]->nom = tmp_nom;
        }
        
        //Sinon permutter la norme est inutile
        if(_array[y]->norme != _array[i]->norme)
        {  
            _array[y]->norme = _array[i]->norme;
            _array[i]->norme =  tmp_norme;
        }
    }
}

//Retourne une grille d'indice de taille arg1 melangée
int* shuffle2(size_t nb_neurone)
{
	int *indices = malloc(sizeof(int)*nb_neurone);
	for(int i = 0; i < nb_neurone; i++)
	{
		indices[i] = i;
	}
	int rand_i;
	for(int i = 0; i < nb_neurone; i++)
	{
		rand_i = rand()%nb_neurone;
		indices[i] += indices[rand_i];
		indices[rand_i] = indices[i]-indices[rand_i];
		indices[i] -= indices[rand_i];
	}
	return indices;
}

void normalisation(data_v** data, data_info* info)
{	
	for(int i = 0; i < info->nb_line; i++)
	{
		for(int y = 0; y < info->nb_data; y++)
		{
			data[i]->vec[y] /= data[i]->norme;
		}
	}
}

//Affichage d'1 données
void printData(data_v* data)
{
	printf(" %.5lf %.5lf %.5lf %.5lf | name: %s | norme: %.1lf \n", data->vec[0], data->vec[1], data->vec[2], data->vec[3], data->nom, data->norme);
}

void printBDD(data_v** data, int size)
{
	int i = 0;
	while(i < size)
	{
		printf("n° %d ", i+1);
		printData(data[i]);
		i++;
	}
}

//Affichage de pointeur de type DOUBLE
void printVec(double* vec, int size)
{
    for(int i = 0; i < size; i++)
        printf("%.3lf ", vec[i]);
    printf("\n\n");
}

//Calcul du nombre de vecteur de données
size_t countLine(FILE* input)
{   
    int g = 0;
    size_t ls = 0;
    char* tmp = malloc(ls);
    while(getline(&tmp, &ls, input) != -1)   
        g++;
    return g;
}

//Retourne le nombre de donnée dans une ligne
size_t countData(FILE* input)
{	
    int g = 0;
    size_t ls = 0;
    char* buffer;
    char* tmp;
    if(getline(&buffer, &ls, input) != -1)
    {
    	while(1)
    	{
   			if(g == 0)
   				tmp = strtok(buffer, ",;:/|");
   			else
   				tmp = strtok(NULL, ",;:/|");
   			if(tmp == NULL)
   				break;
   			else
   			{
   				//printf("\ntmp : %s", tmp);
   				g++;
   			}
		}        
    }
    return g-1;
}

//Stocke les informations d'une ligne dans la structure de données
ssize_t parser(FILE *input, data_v* data, data_info* info)
{
    size_t bufsize = 0;//nb_caractere max d'une ligne
    char* buffer;
    ssize_t line_size;
    line_size = getline(&buffer, &bufsize, input);
    if(line_size == -1)
        return line_size;
    else
    { 
        int i = 0;
        char* c;
        data->vec = malloc(sizeof(double)*info->nb_data);
        //Parser le
        while(i < info->nb_data)
        {
            if(i == 0)
                c = strtok(buffer, ",");
            else
                c = strtok(NULL, ",");
            data->vec[i] = atof(c);
            i++; 
        }   
        //Parser le nom 
        c = strtok(NULL, "\n");  
        data->nom = c; 
    }
    return line_size;
}

//Initialisation base de données
void initData(data_v** iris_data, data_info* info, FILE* input)
{
    size_t i = 0;
	printf("Initialisation de la base de donnees\n");
	iris_data[0] = malloc(sizeof(data_v));
	while(parser(input, iris_data[i], info) != -1)
	{
        iris_data[i]->norme = norme(iris_data[i]);
	    i++;
	    iris_data[i] = malloc(sizeof(data_v));
	}
}
