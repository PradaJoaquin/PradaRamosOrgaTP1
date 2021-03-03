#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cache.h"

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
        size_t tag;    //identifica el bloque.
        void* data;   //la unidad minima es de 2 bytes, = void dato[tope];
        size_t ins;  //Almacena la instruccion que usa el bloque (la linea del archivo).
    }bloque_t;

typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E;             // Cantidad de bloques = "E"
	bloque_t* bloques; 	 // = bloque_t bloques[]
}set_t;

struct cache
{
    int time;  // Se usa para el last recently used, numero de operacion actual. //SE PODRIA SACAR.
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
            cache->sets[i].bloques[k].es_valido = 0;
            cache->sets[i].bloques[k].dirty_bit = 0;
            cache->sets[i].bloques[k].tag = 0;
            cache->sets[i].bloques[k].data = malloc(sizeof(tam_bloque));
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

op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos){
    op_result_t* result = malloc(sizeof(op_result_t));
    if(!result){
        return NULL;
    }


    return result;
}

/*
    typedef struct bloque
    {
        bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
        bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
        size_t tag;    //identifica el bloque.
        void* data;   //la unidad minima es de 2 bytes, = void dato[tope];
        size_t ins;  //Almacena la instruccion que usa el bloque (la linea del archivo).
    }bloque_t;

    typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
    {
        size_t E;             // Cantidad de bloques = "E"
        bloque_t* bloques;   // = bloque_t bloques[]
    }set_t;

    struct cache
    {
        int time;  // Se usa para el last recently used, numero de operacion actual. //SE PODRIA SACAR.
        size_t S; // Parametro "S" cantidad de sets
        set_t* sets;
    };
*/

bloque_t* buscar_menores(bloque_t** bloques, size_t tope)
{
    size_t ins_menor = *(bloques[0])->ins; //todas son positivas.
    size_t pos_menor = 0;

    for (int i = 1; i < tope; ++i)
    {
        if( *(bloques[i])->ins < ins_menor){
            //guardo la menor instruccion.
            ins_menor = *(bloques[i])->ins;
            //guardo el puntero del bloque en el auxiliar.
            pos_menor = i;
        }
    }
    return *(bloques[pos_menor]); //puntero al bloque.
}


/*
*   Dado una matriz cache devuelve el bloque menos usado, siempre devuelve uno. 
*/
bloque_t* encontrar_LRU(cache_t* cache)
{
    /* Busco los menores por sets, los guardo, y por ultimo devuelvo el menor de los menores. */

    //creo el contenedor auxiliar.
    bloque_t** menores[cache->S]; //vector de punteros de bloque_t*
    size_t tope = cache->S;
    size_t tope_bloques = cache->set->E;

    for (int i = 0; i < tope; ++i)
    {
        // voy buscando los menores; cache->sets[]->bloques[].
        menores[i] = buscar_menores(&(cache->sets[i]->bloques), tope_bloques);
    }

    //ahora busco el menor de los menores.
    return buscar_menores(menores, tope);
}