#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "simulador.h"
#include "cache.h"
#include "cacheutil.h"
#include "estadisticas.h"

#define CLEAN_MISS "2a"
#define DIRTY_MISS "2b"

struct simulador
{
    cache_t* cache;
    size_t modo_verb_restante;
};

simulador_t* simulador_crear(size_t cache_tam, size_t cache_asociatividad, size_t cache_sets, size_t ini, size_t fin){
    simulador_t* sim = malloc(sizeof(simulador_t));
    if(!sim){
        return NULL;
    }
    sim->cache = cache_crear(cache_tam, cache_asociatividad, cache_sets);
    if(!sim->cache){
        free(sim);
        return NULL;
    }
    sim->modo_verb_restante = fin - ini;
    return sim;
}

void simulador_destruir(simulador_t* sim){
    cache_destruir(sim->cache);
    free(sim);
}

/*
    typedef struct op_result
    {
        char operacion;         // w o r
        resultados_t resultado;// Hit, clean miss o dirty miss.
        addr_t direccion;     // tag, indice
        size_t instruccion;  // podemos guardar aca la linea del archivo.
        bool valido;        // Indica si se cargo un dato en la memoria o no. 
        bool dirty_bit;    // cambia el curso de algunas operaciones. 
        size_t ant_tag;   // -1 por default, para modo verboso.
        size-t ant_bloque_ins; //anterior bloque->ins, para modo verboso.
    }op_result_t;
*/

/*
    1_ n (ok)
    2_ id caso : hit = 1, clean_miss = 2a, dirty miss = 2b; (ok result->resultado)
    3_ Indice de cache (0 a S). (S)                         (ok addr index)
    4_ cache tag, direccion de la operacion en hexadecimal. (ok addr tag)
    5_ (E)                                                  (ok addr ofset)
    6_ tag anterior que habia en el bloque, o -1 si no hay. (ok addr tag, antes de escribir)
    7_ valid_bit                                            (ok result)
    8_ dirty bit                                            (ok result)
    9_ lru local (debo buscar el menor siempre).            (ok funcion cache).(se debe agregar a operar)
*/
void simulador_modo_verboso(op_result_t* result, size_t instruccion){
	//0 2a 36 2feee4 0 -1 0 0 0
    printf("%ld ", instruccion);
    if(result->resultado == hit) printf("%d ", HIT);
    else if(result->resultado == clean_miss) printf("%s ", CLEAN_MISS);
    else printf("%s ", DIRTY_MISS);

    printf("%ld ", result->direccion.index);
    printf("%lx ", result->direccion.tag);  
    printf("%ld ", result->direccion.off);
    
    printf("%lx ", result->ant_tag);  //-1 por default, necesito la etiqueta anterior
    printf("%d ", result->valido);
    printf("%d ", result->dirty_bit);
    printf("%ld\n", result->ant_bloque_ins); 
}

//simulador debe recibir estadisticas si solo hace una operacion a la vez.
void simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t instruccion, 
    estadisticas_t* estadisticas){
//op_result_t* cache_operar(cache_t* cache, char op, size_t dir, size_t tam, size_t datos);
    op_result_t* result = cache_operar(sim->cache, *operacion, direccion, instruccion);
    if(!result) return;

    if(sim->modo_verb_restante > 0){
        simulador_modo_verboso(result, instruccion);
        sim->modo_verb_restante--;
    }
    //comienzo de operaciones.


    // Aca se deberian actualizar las estadisticas totales con lo que devuelva cache_operar
    //cuando se carga op_result
    cargar_estadisticas(estadisticas, result);
    free(result);
}