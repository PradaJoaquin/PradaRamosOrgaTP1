#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cache.h"

#define BYTE_SIZE 8

#define ESCRITURA 'W'
#define LECTURA 'R'

typedef struct bloque
{
    size_t tam; // Tamanio del bloque en bytes

	bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
	bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
	size_t tag;    //identifica el bloque.
	void* data;   //la unidad minima es de 2 bytes, = void dato[tope];
    size_t ins;  //Guarda la instruccion que usa el bloque (puede ser la linea del archivo).
}bloque_t;


//------------------------------------TESTEADO--------------------------------------------
void destruir_bloques(bloque_t* bloque, size_t tope);

void cache_destruir_hasta(cache_t* cache, size_t tope);
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
        bloque[i].tam = tam_bloque;
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

    cache_t* cache = malloc(sizeof(cache_t));  
    if(!cache) return NULL;
    
    cache->S = num_sets;
    cache->sets = calloc(num_sets, sizeof(set_t)); //asi se inicializa. 
    if(!cache->sets){
        free(cache);
        return NULL;
    }

    for(size_t i = 0; i < num_sets; i++)
    {
        cache->sets[i].E = asociatividad;
        cache->sets[i].bloques = crear_bloques(asociatividad, tam_bloque); //aca el set queda inicializado. 
        if(!cache->sets[i].bloques){
            cache_destruir_hasta(cache, i); //destruye hasta la posicion indicada.
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

void cache_destruir(cache_t* cache){
    for(size_t i = 0; i < cache->S; i++){
        for(size_t k = 0; k < cache->sets[i].E; k++){
            free(cache->sets[i].bloques[k].data);
        }
        free(cache->sets[i].bloques);
    }
    free(cache->sets);
    free(cache);
}


/*
*   Desrtuye hasta la posicion indicada por max, si se desea destruir todo, debe pasarse la cantidad de sets.
**  Se implemento de esta forma para manejar los errores de memoria al crear, optimizando la reutilizacion de codigo.
*/
void cache_destruir_hasta(cache_t* cache, size_t tope)
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
            destruir_bloques(cache->sets[i].bloques, cache->sets[i].E);   
        else free( &(cache->sets[i]) );                                  
    } 

    free(cache->sets); 
    free(cache);      
} 
//----------------------------------------------------------------------------------------

/*
*   Dado una matriz cache devuelve el bloque menos usado del set (local), siempre devuelve uno. 
*/
bloque_t* encontrar_LRU(bloque_t* bloques, size_t tope)
{
    size_t ins_menor; //todas son positivas.
    long int pos_menor = -1;

    for (size_t i = 0; i < tope; ++i)
    {
        if(!bloques[i].es_valido){ // Si encuentra un bloque no valido, entonces lo devulve
            return &bloques[i];
        }
        if(pos_menor == -1 || bloques[i].ins < ins_menor){
            ins_menor = bloques[i].ins; //guardo la menor instruccion.
            pos_menor = i;   //guardo el puntero del bloque en el auxiliar.
        }
    }
    return &bloques[pos_menor]; //puntero al bloque.
}

addr_t addr_crear(size_t dir, size_t block_size, size_t num_sets){
    addr_t addr;
    addr.off = dir & (block_size - 1);
    size_t index = (dir >> ((int)log2((double)block_size)));
    addr.index = index & (num_sets - 1);
    addr.tag = index >> ((int)log2((double)num_sets));
    return addr;
}

void configurar_remplazo(bloque_t* remplazo, char op, addr_t addr, size_t instruccion){
    remplazo->es_valido = true;
    remplazo->tag = addr.tag;
    remplazo->ins = instruccion;
    remplazo->dirty_bit = false;
    if(op == ESCRITURA){
        remplazo->dirty_bit = true;
    }
}

//warning datos sin usar.
//debo capturar la instruccion anterior que tenia el bloque, antes de actualizarlo, para el modo verboso.
op_result_t* cache_operar(cache_t* cache, char op, size_t dir, size_t instruccion){
    
    op_result_t* result = malloc(sizeof(op_result_t));
    if(!result) return NULL;
    
    addr_t addr = addr_crear(dir, cache->sets->bloques->tam * BYTE_SIZE, cache->S);

    result->operacion = op;
    result->direccion = addr;
    result->instruccion = instruccion;

    set_t set = cache->sets[addr.index];
    for(size_t i = 0; i < set.E; i++){
        if(set.bloques[i].es_valido && set.bloques[i].tag == addr.tag){
            // HIT
            result->ant_bloque_ins = set.bloques[i].ins; //para el modo verboso.
            set.bloques[i].ins = instruccion;

            if(op == ESCRITURA){
                set.bloques[i].dirty_bit = true;
            }

            result->resultado = hit;
            return result;
        }
    }
    // MISS
    bloque_t* remplazo = encontrar_LRU(set.bloques, set.E);
    if(!remplazo->es_valido || !remplazo->dirty_bit){
        // Clean cache miss
        configurar_remplazo(remplazo, op, addr, instruccion);
        result->resultado = clean_miss;
    }else{
        // Dirty cache miss
        configurar_remplazo(remplazo, op, addr, instruccion);
        result->resultado = dirty_miss;
    }

    return result;
}