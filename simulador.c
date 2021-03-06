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
    size_t cache_tam;
    size_t cache_asociatividad;
    size_t cache_sets;

    size_t modo_verb_inicio;
    size_t modo_verb_final;

    estadisticas_t* estadisticas;
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

    sim->estadisticas = estadisticas_crear();
    if(!sim->estadisticas){
        cache_destruir(sim->cache);
        free(sim);
        return(NULL);
    }

    sim->modo_verb_final = fin;
    sim->modo_verb_inicio = ini;

    sim->cache_tam = cache_tam;
    sim->cache_asociatividad = cache_asociatividad;
    sim->cache_sets = cache_sets;
    
    return sim;
}

void simulador_destruir(simulador_t* sim){
    if(!sim) return;
    cache_destruir(sim->cache);
    estadisticas_destruir(sim->estadisticas);
    free(sim);
}


void simulador_modo_verboso(op_result_t* result, size_t instruccion, size_t asociatividad){

    printf("%ld ", instruccion);                                            
    if(result->resultado == hit) printf("%d ", HIT);
    else if(result->resultado == clean_miss) printf("%s ", CLEAN_MISS);     
    else printf("%s ", DIRTY_MISS);                                         

    printf("%lx ", result->direccion.index);                                
    printf("%lx ", result->direccion.tag);                                 
    printf("%ld ", result->op_bloque_off);                                   
    
    //Para estos casos en los que no se escriben datos, la etiqueta anterior es igual a la actual.
    if(result->valido == 1) printf("%lx ", result->direccion.tag);          
    else  printf("-1 ");

    if(result->valido) printf("1 ");                                       
    else printf("0 ");

    if(result->dirty_bit) printf("1 ");                                    
    else printf("0 ");

    if(asociatividad > 1)
        printf("%ld", result->ant_bloque_ins);                            
    printf("\n");
}

//simulador debe recibir estadisticas si solo hace una operacion a la vez.
void simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t instruccion){

    if(!sim){
        return;
    }
    
    op_result_t* result = cache_operar(sim->cache, *operacion, direccion, instruccion);
    if(!result) return;

    if(instruccion == sim->modo_verb_inicio && instruccion <= sim->modo_verb_final && sim->modo_verb_final != 0 ){
        simulador_modo_verboso(result, instruccion, sim->cache_asociatividad);
        sim->modo_verb_inicio++; //avanzo inicio junto con la instruccion.
    }

    //Aca se actualizan las estadisticas totales con lo que devuelva cache_operar.
    cargar_estadisticas(sim->estadisticas, result);
    free(result);
}

void simulador_imprimir_estadisticas(simulador_t* sim){
    imprimir_estadisticas(sim->estadisticas, sim->cache_sets, sim->cache_asociatividad, sim->cache_tam);
}