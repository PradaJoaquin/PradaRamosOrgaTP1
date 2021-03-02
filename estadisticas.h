#ifndef __ESTADISTICAS_H__
#define __ESTADISTICAS_H__ 

#define HIT 1
#define PENALTY 100
#define KB 1024

#include <stdio.h>

typedef struct cache cache_t;

typedef struct estadisticas
{
	size_t lecturas;	    //se contabilizan siempre.
	size_t escrituras;     //lecturas + escrituras = N instrucciones.
	size_t clean_miss;    //Se activa cuando dirty bit = 0.
	size_t dirty_rmiss;  //Se contabiliza cuando rmiss + dirty bit = 1.
	size_t dirty_wmiss; //Se contabiliza cuando wmiss + dirty bit = 1.
	size_t rmiss;	   //rmiss + wmiss = total miss.
	size_t wmiss;
}estadisticas_t;

/*
*  bytes written = ((escrituras + wmiss) * unidad_datos)/8. //mal
*  bytes read = ((lecturas + rmiss) * unidad_datos)/8. //mal
*  miss rate = total miss / (lecturas + escrituras).
*  capacidad cache = S x E x Unidad Datos.
*  bytes read = total misses x tamanio bloque. (tamanio bloque = cache size /(S*E)).
*  bytes written = (dirty rmiss + dirty wmiss) * tamanio bloque.
*/

/*penalizaciones (ciclos)
*
*   clean miss = 1 + penalty; (clean miss = rmiss + wmiss)
*   dirty miss = 1 + 2*penalty; (dirty miss = d rmiss + s w miss)
*  	readtime =  (rmiss + dirty_rmiss) * penalty + lecturas;  (1 lectura = 1 ciclo)
*  	writetime = (wmiss + dirty wmiss) *  penalty + escrituras;
*/

/*
* Recibe el tamanio_cache como cantidad de bytes, los sets y la accesibilidad deben estar validados.
* Imprime y calcula todas las estadisticas relevantes del el archivo:
*	lectuas, escrituras, clean misses, dirty misses, bytes leidos y bytes escritos.
*/
void imprimir_estadisticas(estadisticas_t* estadisticas, size_t sets, size_t E, size_t tamanio_cache)

#endif //__ESTADISTICAS_H__ 