//Réseau de neuronne Self Organizing Map
//formulas.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "data.h"

//fonction moyenne des vecteurs de données (ici iris_data)
double* average(data_v **data, int nb_line)
{
    //Calcul du vecteur moyen
	double* ave = malloc(sizeof(double) * 4);
    
	for(int i = 0; i < 4; i++)
	{
	    ave[i] = 0;
	    for(int y = 0; y < nb_line; y++)
		    ave[i] += data[y]->vec[i];
		ave[i] = ave[i]/nb_line;
	}
    return ave;
}

/*			____________
   F(x) : \/(w²+x²+y²+z²)\ */
double norme(data_v *data)
{
	double n = 0.0f;
	for(int i = 0; i < 4; i++)	
		n += (data->vec[i] * data->vec[i]);
	return sqrt(n);
}

/*   	       ___________________________________________________
   F(p,q) :  \/ (|p0-q0|)² + (|p1-q1|)² + (|p2-q2|)² + (|p3-q3|)² \ 
   TEMP FCT*/
double euclidean_distance(double* data_vec, double* neurone_vec)
{	
    double n = 0;
    for(int i = 0; i < 4; i++)
        n += pow(fabs(data_vec[i]- neurone_vec[i]), 2);
        
	return sqrt(n);
}

//DONE
double rand_double(double n, double min, double max)
{
    double max_n = n + max;
    double min_n = n - min; 
	return (rand()/(double)RAND_MAX) * (max_n-min_n) + min_n;
} 

int rand_int(int min, int max)
{
	int rand_value = (int)rand() / RAND_MAX;
	return min + rand_value * (max - min);
}
