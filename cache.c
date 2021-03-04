#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cache.h"

#define TAM_DIRECCION_MEMORIA 32

typedef struct bloque
{
    size_t tam; // Tamanio del bloque en bytes

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
    int k = 0;
    for(int i = 0; i < num_sets; i++){
        cache->sets[i].E = asociatividad;
        cache->sets[i].bloques = malloc(sizeof(bloque_t) * asociatividad);
        if(!cache->sets[i].bloques){
            for(int j = 0; j < i; j++){
                for(int q = 0; q < k; q++){
                    free(cache->sets[j].bloques[q].data);
                }
                free(cache->sets[j].bloques);
            }  
            free(cache->sets);
            free(cache);
            return NULL;
        }
        for(k = 0; k < asociatividad; k++){
            cache->sets[i].bloques[k].tam = tam_bloque;
            cache->sets[i].bloques[k].es_valido = 0;
            cache->sets[i].bloques[k].dirty_bit = 0;
            cache->sets[i].bloques[k].tag = 0;
            cache->sets[i].bloques[k].data = malloc(tam_bloque);
            if(!cache->sets[i].bloques[k].data){
                for(int j = 0; j < i; j++){
                    for(int q = 0; q < k; q++){
                        free(cache->sets[j].bloques[q].data);
                    }
                    free(cache->sets[j].bloques);
                }
                free(cache->sets);
                free(cache);
                return NULL;
            }
        }
    }
    return cache;
}

void cache_destruir(cache_t* cache){
    for(int i = 0; i < cache->S; i++){
        for(int k = 0; k < cache->sets[i].E; k++){
            free(cache->sets[i].bloques[k].data);
        }
        free(cache->sets[i].bloques);
    }
    free(cache->sets);
    free(cache);
}

addr_t addr_crear(size_t dir, size_t block_size, size_t num_sets){
    addr_t addr;
    addr.off = dir & (block_size - 1);
    addr.index = (dir >> (int)log2((double)block_size) - 1) & (num_sets - 1);
    addr.tag = dir >> ((int)log2((double)block_size) - 1 + (int)log2((double)num_sets) - 1);
    return addr;
}

op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos){
    op_result_t* result = malloc(sizeof(op_result_t));
    if(!result){
        return NULL;
    }
    addr_t addr = addr_crear(dir, cache->sets->bloques->tam * 8, cache->S); // Corregir esto despues.. (el sizeof esta mal)

    printf("%zu\n", addr.index);

    result->operacion = op[0];
    result->direccion = addr;
    /*
    set_t set = cache->sets[addr.index];
    for(int i = 0; i < set.E; i++){
        if(set.bloques[i].tag == addr.tag){
            // HIT
            result->resultado = // HIT
        }
    }
    // MISS
    bloque_t remplazo = encontrar_LRU(cache); // revisar esta funcion
    */



    return result;
}
