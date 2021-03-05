#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>
#include "cacheutil.h"

#define HIT 1
#define PENALTY 100 

typedef struct cache cache_t;

/*
*	Pre: los parametros fueron validados de antemano.
*	Dado los argumentos del programa, crea una matriz cache inicializada y devuelve su puntero
* 	o NULL en caso de que no se pueda crear.
*/
cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets);


/*
*   Desrtuye hasta la posicion indicada por tope, si se desea destruir todo, debe pasarse la cantidad de sets.
*/
void cache_destruir(cache_t* cache);

/*
*	Realiza una operacion de la cache, la procesa y devuelve un registro con datos relevantes de la operacion.
* 	En caso de fallo devuelve NULL.
*/
op_result_t* cache_operar(cache_t* cache, char op, size_t dir, size_t instruccion);

#endif