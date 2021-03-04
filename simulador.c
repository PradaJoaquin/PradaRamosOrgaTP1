#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "simulador.h"
#include "cache.h"
#include "cacheutil.h"


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
    cache_destruir(sim->cache,  sim->cache->S);
    free(sim);
}

void simulador_modo_verboso(op_result_t* result){
    // Aca se recibe el struct que devuelve cache con todos las estadisticas suficientes para imprimir el modo verboso.
}

//simulador debe recibir estadisticas si solo hace una operacion a la vez.
void simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t tam, size_t datos, 
    estadisticas_t* estadisticas){

    op_result_t* result = cache_operar(sim->cache, *operacion, direccion, tam, datos);
    if(!result) return;

    if(sim->modo_verb_restante > 0){
        simulador_modo_verboso(result);
        sim->modo_verb_restante--;
    }

    // Aca se deberian actualizar las estadisticas totales con lo que devuelva cache_operar
    //cuando se carga op_result
    cargar_estadisticas(estadisticas, result);
    free(result);
}