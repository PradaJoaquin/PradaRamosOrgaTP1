#ifndef __ESTADISTICAS_H__
#define __ESTADISTICAS_H__ 

#define HIT 1
#define PENALTY 100
#define KB 1024

#include <stdio.h>

typedef enum resultado = {hit, clean_miss, dirty_rmiss} resultado_t;
						 //0   ,    1     ,    2
typedef struct addr addr_t;

typedef struct op_result
{
    char operacion; 		// w o r
    resultado_t resultado; // Hit, clean miss o dirty miss.
    addr_t direccion;	  // tag, indice?
    size_t instruccion;  // podemos guardar aca la linea del archivo.
    bool valido; 		// Indica si se cargo un dato en la memoria o no.
    bool dirty_bit     // cambia el curso de algunas operaciones.
}op_result_t;

typedef struct estadisticas
{
	size_t lecturas;	     //se contabilizan siempre.
	size_t escrituras;      //lecturas + escrituras = N instrucciones.
	size_t rmiss;    	   //Se activa cuando dirty bit = 0.
	size_t wmiss
	size_t dirty_rmiss;  //Se contabiliza cuando rmiss + dirty bit = 1.
	size_t dirty_wmiss; //Se contabiliza cuando wmiss + dirty bit = 1.
	size_t rmiss;	   //rmiss + wmiss = total miss.
	size_t wmiss;
}estadisticas_t;

/*
*  capacidad cache = S x E x Unidad Datos.
*/

/*
*	Recibe la operacion procesada y carga los resultados en la estadistica pasada por parametro.
*/
void cargar_estadisticas(estadisticas_t* estadisticas, op_result_t* op_result);

/*
*  Recibe el tamanio_cache como cantidad de bytes, los sets y la accesibilidad deben estar validados.
*  Imprime y calcula todas las estadisticas relevantes del el archivo:
*  lectuas, escrituras, clean misses, dirty misses, bytes leidos y bytes escritos.
*/
void imprimir_estadisticas(estadisticas_t* estadisticas, size_t sets, size_t E, size_t tamanio_cache)

#endif //__ESTADISTICAS_H__ 