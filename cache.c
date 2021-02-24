#include <stdlib.h>
#include <stdio.h>
#include "cache.h"

struct bloque
{
	bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
	bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
	size_t tag;   //identifica el bloque.
	void* data; //la unidad minima es de 2 bytes, = void dato[tope];
};

struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E; // Cantidad de bloques = "E"
	bloque_t* bloques; 	   // = bloque_t bloques[]
};

struct cache
{
    size_t S; // Parametro "S"
    set_t* sets;
};