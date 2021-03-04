#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>
#include "cacheutil.h"

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct cache cache_t;
typedef struct bloque bloque_t;

/*
typedef struct estadisticas
{
	size_t lecturas;	     //se contabilizan siempre.
	size_t escrituras;      //lecturas + escrituras = N instrucciones.
	size_t hits;		   //se dan si coinciden (set, tag y validate bit).
	size_t clean_miss;    //Se activa cuando dirty bit = 0.
	size_t dirty_rmiss;  //Se contabiliza cuando rmiss + dirty bit = 1.
	size_t dirty_wmiss; //Se contabiliza cuando wmiss + dirty bit = 1.
	size_t rmiss;	   //rmiss + wmiss = total miss.
	size_t wmiss;
}estadisticas_t;
*/

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
*   Destruye la cache.
*/
void cache_destruir(cache_t* cache);

/*
*   Dado una matriz cache devuelve el bloque menos usado del set (local), siempre devuelve uno. 
*/
bloque_t* encontrar_LRU(bloque_t** bloques, size_t tope);

op_result_t* cache_operar(cache_t* cache, char op, size_t dir, size_t tam, size_t datos);

#endif