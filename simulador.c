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

simulador_t* simulador_crear(size_t cache_tam, size_t cache_asociatividad, size_t cache_sets, char* modo_verboso, size_t ini, size_t fin){
    simulador_t* sim = malloc(sizeof(simulador_t));
    if(!sim){
        return NULL;
    }
    sim->cache = cache_crear(cache_tam, cache_asociatividad, cache_sets);
    if(!sim->cache){
        free(sim);
        return NULL;
    }
    if(modo_verboso){
        sim->modo_verb_restante = fin - ini;
    }else{
        sim->modo_verb_restante = 0;
    }

    return sim;
}


