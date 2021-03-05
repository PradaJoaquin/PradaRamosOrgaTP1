#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>
#include "cacheutil.h"

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct cache cache_t;
typedef struct bloque bloque_t;

//Simulador debe conocer cache y set, por E y S, y bloques que se acceden en el .c
typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E; // Cantidad de bloques = "E"
	bloque_t* bloques; 	   // = bloque_t bloques[]
}set_t;

typedef struct cache
{
    int time; // Se usa para el last recently used, numero de operacion actual. 
    size_t S; // Parametro "S" cantidad de sets
    set_t* sets;
}cache_t;



/*
*	Dado los argumentos del programa, crea una matriz cache inicializada y devuelve su puntero
* 	o NULL en caso de que no se pueda crear.
*/
cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets);


/*
*   Desrtuye hasta la posicion indicada por tope, si se desea destruir todo, debe pasarse la cantidad de sets.
*/
void cache_destruir(cache_t* cache);


op_result_t* cache_operar(cache_t* cache, char op, size_t dir, size_t instruccion);

#endif