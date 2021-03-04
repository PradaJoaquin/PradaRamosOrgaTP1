#ifndef __SIMULADOR_H__
#define __SIMULADOR_H__ 

#include "estadisticas.h" //o juntar estadisticas con simulador...

typedef struct simulador simulador_t;
typedef struct estadisticas estadisticas_t;

simulador_t* simulador_crear(size_t cache_tam, size_t cache_asociatividad, size_t cache_sets, size_t ini, size_t fin);

void simulador_destruir(simulador_t* sim);

void simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t tam, size_t datos, size_t instruccion, estadisticas_t* estadisticas);

#endif