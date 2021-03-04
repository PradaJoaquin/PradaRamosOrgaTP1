#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cache.h"


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


void destruir_bloques(bloque_t* bloque, size_t tope);

/*
*   Dada la cantidad de bloques, crea un vector con los bloques necesarios, inicializados y 
*   devuelve el puntero al mismo, en caso de fallar devuelve NULL.
*/
bloque_t* crear_bloques(size_t asociatividad, size_t tam_bloque)
{   
    bloque_t* bloque = calloc(asociatividad, sizeof(bloque_t)); 
    if(!bloque) return NULL;

    for(size_t i = 0; i < asociatividad; ++i)
    {
        bloque[i].data = malloc(tam_bloque); //puede contener basura.
        if(!bloque[i].data){                //debo destruir los anteriores.
            destruir_bloques(bloque, i);
            return NULL;
        }else bloque[i].tag = i;         //inicializa el tag, son todos sucesivos van de 0 a i(E -1).
    }
    return bloque; //todos los parametros restantes son 0 por calloc.
}

cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets)
{
    size_t tam_bloque = tam / (asociatividad * num_sets);

    cache_t* cache = malloc(sizeof(cache_t));   //1
    if(!cache) return NULL;
    
    cache->S = num_sets;
    cache->sets = calloc(num_sets, sizeof(set_t)); //asi se inicializa. //64
    if(!cache->sets){
        free(cache);
        return NULL;
    }else cache->S = num_sets;

    for(size_t i = 1; i < num_sets; i++)
    {
        cache->sets[i].E = asociatividad;
        cache->sets[i].bloques = crear_bloques(asociatividad, tam_bloque); //aca el set queda inicializado. //128
        if(!cache->sets[i].bloques){
            cache_destruir(cache, i); //destruye hasta la posicion indicada.
            return NULL;
        }
    }
    return cache;
}


void destruir_bloques(bloque_t* bloque, size_t tope)
{
    for (size_t i = tope-1; i > 0; --i)
        free(bloque[i].data);
    free(bloque->data);
    free(bloque);
}

/*
*   Desrtuye hasta la posicion indicada por max, si se desea destruir todo, debe pasarse la cantidad de sets.
**  Se implemento de esta forma para manejar los errores de memoria al crear, optimizando la reutilizacion de codigo.
*/
void cache_destruir(cache_t* cache, size_t tope)
{
    if(!cache) return;
    if(!cache->sets){
        free(cache);
        return;
    } 

    for(size_t i = tope -1; i > 0; i--)
    {
        if(cache->sets[i].bloques != NULL)
            //existen los bloques. ok.
            destruir_bloques(cache->sets[i].bloques, cache->sets[i].E);   //128
        else free( &(cache->sets[i]) );                                  //64
    } 

    free(cache->sets); //1
    free(cache);      //1
} 

/*
op_result_t* cache_operar(cache_t* cache, char* op, size_t dir, size_t tam, size_t datos){
    op_result_t* result = malloc(sizeof(op_result_t));
    if(!result) 
        return NULL;    
    
    return result;
}
*/

/*
*   Dado una matriz cache devuelve el bloque menos usado del set (local), siempre devuelve uno. 
*/
bloque_t* encontrar_LRU(bloque_t** bloques, size_t tope)
{
    size_t ins_menor = bloques[0]->ins; //todas son positivas.
    size_t pos_menor = 0;

    for (size_t i = 1; i < tope; ++i)
    {
        if( bloques[i]->ins < ins_menor){
            ins_menor = bloques[i]->ins; //guardo la menor instruccion.
            pos_menor = i;   //guardo el puntero del bloque en el auxiliar.
        }
    }
    return bloques[pos_menor]; //puntero al bloque.
}
