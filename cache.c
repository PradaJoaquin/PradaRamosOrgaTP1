#include <stdlib.h>
#include <stdio.h>
#include "cache.h"

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

            // Ver como hacer bien para cuando falla esto
            // Ya que habria que iterar nuevamente para freear los que no fallaron

        }
        for(int k = 0; k < asociatividad; k++){
            cache->sets[i].bloques[k].es_valido = 0;
            cache->sets[i].bloques[k].dirty_bit = 0;
            cache->sets[i].bloques[k].tag = 0;
            cache->sets[i].bloques[k].data = malloc(sizeof(tam_bloque));
            if(!cache->sets[i].bloques[k].data){

                // Lo mismo para aca
                
            }
        }
    }
    return cache;
}

void* cache_leer(cache_t* cache, size_t direccion){

}

bool cache_escribir(cache_t* cache, size_t direccion, void* dato){

}