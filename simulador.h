#ifndef __SIMULADOR_H__
#define __SIMULADOR_H__ 

#include <stdlib.h>

typedef struct simulador simulador_t;

/*
*	Pre: los parametros fueron chequeados de antemano.
*	Dados los parametros recibidos por argumento, devuelve un puntero a un simulador, en caso de fallo, devuelve NULL.	
*/
simulador_t* simulador_crear(size_t cache_tam, size_t cache_asociatividad, size_t cache_sets, size_t ini, size_t fin);

/*
*	Destruye un simulador.
*/
void simulador_destruir(simulador_t* sim);

/*
*	Dados un simulador, la operacion y la direccion en la cual operar, simula una sola operacion en de cache, tambien
* 	registra los datos necesarios para las estadisticas.
*/
void simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t instruccion);

/*
*	Imprime las estadisticas acumuladas hasta el momento.
*/
void simulador_imprimir_estadisticas(simulador_t* sim);

#endif