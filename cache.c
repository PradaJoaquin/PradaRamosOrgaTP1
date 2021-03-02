#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include "cacheutil.h"

typedef struct addr
{
	int tag;
	int index;
	int off;
}addr_t;

typedef struct bloque
{
	bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
	bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
	size_t tag;   //identifica el bloque.
	void* data; //la unidad minima es de 2 bytes, = void dato[tope];
}bloque_t;

typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E; // Cantidad de bloques = "E"
	bloque_t* bloques; 	   // = bloque_t bloques[]
}set_t;

struct cache
{
    int time; // Se usa para el last recently used, numero de operacion actual. 
    size_t S; // Parametro "S" cantidad de sets
    set_t* sets;
};

cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets){
    size_t tam_bloque = tam / (asociatividad * num_sets);

    cache_t* cache = malloc(sizeof(cache_t));
    if(!cache){
        return NULL;
    }
    cache->S = num_sets;
    cache->sets = malloc(sizeof(set_t) * num_sets);
    if(!cache->sets){
        free(cache);
        return NULL;
    }
    for(int i = 0; i < num_sets; i++){
        cache->sets[i].E = asociatividad;
        cache->sets[i].bloques = malloc(sizeof(bloque_t) * asociatividad);
        if(!cache->sets[i].bloques){
            free(cache->sets);
            free(cache);
            return NULL;
        }
        for(int k = 0; k < asociatividad; k++){
            cache->sets[i].bloques[k].es_valido = 0;
            cache->sets[i].bloques[k].dirty_bit = 0;
            cache->sets[i].bloques[k].tag = 0;
            cache->sets[i].bloques[k].data = malloc(sizeof(tam_bloque));
            if(!cache->sets[i].bloques[k].data){
                free(cache->sets->bloques);
                free(cache->sets);
                free(cache);
                return NULL;
            }
        }
    }
    return cache;
}

op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos){
    op_result_t* result = malloc(sizeof(op_result_t));
    if(!result){
        return NULL;
    }





    return result;
}
