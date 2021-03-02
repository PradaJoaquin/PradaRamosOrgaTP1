#ifndef __SIMULADOR_H__
#define __SIMULADOR_H__ 

typedef struct simulador simulador_t;

simulador_t* simulador_crear(size_t cache_tam, size_t cache_asociatividad, size_t cache_sets, size_t ini, size_t fin);

void simulador_destruir(simulador_t* sim);


#endif