#ifndef __ESTADISTICAS_H__
#define __ESTADISTICAS_H__ 
#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

#include <stdio.h>

typedef struct cache cache_t;

typedef struct estadisticas
{
	size_t lecturas;	     //se contabilizan siempre.
	size_t escrituras;      //lecturas + escrituras = N instrucciones.
	size_t hits;		   //se dan si coinciden (set, tag y validate bit).
	size_t clean_miss;    //Se activa cuando dirty bit = 0.
	size_t dirty_rmiss;  //Se contabiliza cuando rmiss + dirty bit = 1.
	size_t dirty_wmiss; //Se contabiliza cuando wmiss + dirty bit = 1.
	size_t rmiss;	   //rmiss + wmiss = total miss.
	size_t wmiss;
}estadisticas_t;

//bytes written = ((escrituras + wmiss) * unidad_datos)/8. //mal

//bytes read = ((lecturas + rmiss) * unidad_datos)/8. //mal

//miss rate = total miss / (lecturas + escrituras).

//capacidad cache = S x E x Unidad Datos.

//penalizaciones (ciclos)
//
//  clean miss = 1 + penalty; (clean miss = rmiss + wmiss)
//  dirty miss = 1 + 2*penalty; (dirty miss = d rmiss + s w miss)
//  	readtime =  (rmiss + dirty_rmiss) * penalty + lecturas;  (1 lectura = 1 ciclo)
//  	writetime = (wmiss + dirty wmiss) *  penalty + escrituras;
// 		bytes read = (lecturas + rmiss + dirty_rmiss) * tamanio_bloque; //mal
// 		bytes written = ((escrituras) * tamanio_bloque)/8; 				//mal

size_t penalizacion_rmisses(size_t rmiss);


size_t penalizacion_wmisses(size_t wmiss);


size_t penalizacion_dirty_rmisses(size_t dirty_rmiss);


size_t penalizacion_dirty_wmisses(size_t dirty_wmiss);


size_t calcular_readtime(estadisticas_t* estadisticas);


size_t calcular_writetime(estadisticas_t* estadisticas);


//tam = 2048/(128) = 2 bytes por bloque --> 
size_t calcular_bytes_written(estadisticas_t* estadisticas, size_t tamanio_bloque);


size_t calcular_bytes_read(estadisticas_t* estadisticas, size_t tamanio_bloque);

#endif //__ESTADISTICAS_H__ 