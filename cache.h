#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct bloque
{
	bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
	bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
	size_t tag;   //identifica el bloque.
	void* data; //la unidad minima es de 2 bytes, = void dato[tope];
}bloque_t;

typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E; // Cantidad de bloques = "E"
	bloque_t* bloques; 	   // = bloque_t bloques[]
}set_t;

typedef struct cache
{
    size_t S; // Parametro "S"
    set_t* sets;
}cache_t;

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

//bytes written = (escrituras + wmiss) * unidad_datos.
//bytes read = (lecturas + rmiss) * unidad_datos.
//miss rate = total miss / (lecturas + escrituras).
//capacidad cache = S x E x Unidad Datos.
#endif