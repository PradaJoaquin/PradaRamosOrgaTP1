#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>

#include "estadisticas.h"

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct cache cache_t;
typedef struct bloque bloque_t;
/*
*	Dado los argumentos del programa, crea una matriz cache inicializada y devuelve su puntero
* 	o NULL en caso de que no se pueda crear.
*/
cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets);


/*
*   Desrtuye hasta la posicion indicada por tope, si se desea destruir todo, debe pasarse la cantidad de sets.
**  Se implemento de esta forma para manejar los errores de memoria al crear, optimizando la reutilizacion de codigo.
*/
void cache_destruir(cache_t* cache, size_t tope);

/*
*   Dada una matriz cache devuelve el bloque menos usado, siempre devuelve un bloque. 
*/
bloque_t* encontrar_LRU(cache_t* cache);


op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos);

#endif