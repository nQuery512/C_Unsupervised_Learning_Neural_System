//Réseau de neuronne Self Organizing Map
//data.h
#ifndef H_DATA
#include <math.h>
#include <sys/types.h>
#define H_DATA

//1. sepal length in cm 
//2. sepal width in cm 
//3. petal length in cm 
//4. petal width in cm 
typedef struct data_v
{
	double *vec; //							 ___________			
	double norme;//longueur d'un vecteur : \/w²+x²+y²+z²
	char *nom;
}data_v;

typedef struct data_info
{
    int nb_line; // Nombre de ligne dans le fichier d'entrée
    int nb_data; // Nombre de données par ligne
    int nb_neurone; // Nombre de neurones
   
}data_info;

size_t countLine(FILE*);
size_t countData(FILE*);
void initData(data_v**, data_info*, FILE*);
void normalisation(data_v**, data_info*);
void printBDD(data_v**, int);
void printData(data_v*);
void printVec(double*, int);
ssize_t parser(FILE*, data_v*, data_info*);
void shuffle(data_v**, int);
int * shuffle2(size_t);


#endif
