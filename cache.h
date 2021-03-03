#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct cache cache_t;

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


cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets);

void cache_destruir(cache_t* cache);

/*
*   Dado una matriz cache devuelve el bloque menos usado, siempre devuelve un bloque. 
*/
bloque_t* encontrar_LRU(cache_t* cache);

op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos);

#endif