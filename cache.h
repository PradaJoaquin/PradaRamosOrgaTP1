#ifndef __CACHE_H__
#define __CACHE_H__ 
#include <stdbool.h>
#include <stdio.h>

#define HIT 1
#define PENALTY 100 //o 101 con el siguiente HIT. 

typedef struct cache cache_t;

#define HIT 1
#define CLEAN_MISS 2
#define DIRTY_MISS 3
/*
pub struct OpResult {
    optype: OpType,
    result: Lookup,  // Hit, clean miss, dirty miss.
    addr: Addr,     // Unpacked address (index, tag).
    line_num: usize,
    valid_bit: u8,
    dirty_bit: u8,
    prev_tag: Option<u32>,      // Only present if line was valid.
    prev_mtime: Option<usize>, // Only present if E > 1.
}
*/

typedef enum resultado = {HIT, CLEAN_MISS, DIRTY_MISS} resultado_t;

typedef struct addr
{
    int tag;
    int index;
    int off;
}addr_t;

typedef struct resultado_op
{
    char operacion;
    resultado_t resultado;
    addr_t direccion;
    size_t instruccion;    //indica el numero e linea.
    bool valid_bit;
    bool dirty-bit;
    size_t prev_tag;    //???
    size_t prev_mtime; //???
}resultado_op_t;

//Empaqueta los argumentos necesarios para pasar el struct al cache. 
typedef struct linea
{
    size_t instruccion;       //ID instruccion.
    char operacion;          //Operacion w o r.
    size_t direccion;       //Direccion de 32 bits en la que se hace el acceso.
    int bytes_escritos;    //Indica la cantidad de bytes a escribir/leer.
    size_t datos          //Indica la el dato a operar.
}linea_t;


typedef struct bloque
{
	bool es_valido;  //se activa cuando se realiza una escritura por primera vez en ese set/tag.
	bool dirty_bit; //se activa cuando se produce un miss y se realiza una escritura.
	size_t tag;    //identifica el bloque.
	void* data;   //la unidad minima es de 2 bytes, = void dato[tope];
}bloque_t;

typedef struct set //fila de bloques, la cantidad de bloques queda definida por el archivo
{
    size_t E; // Cantidad de bloques = "E"
	bloque_t* bloques; 	   // = bloque_t bloques[]
}set_t;

struct cache
{
    int time; // Se usa para el last recently used, numero de operacion actual. 
    size_t S; // Parametro "S" cantidad de sets
    set_t* sets;
};

//bytes written = (escrituras + wmiss) * unidad_datos.
//bytes read = (lecturas + rmiss) * unidad_datos.
//miss rate = total miss / (lecturas + escrituras).
//capacidad cache = S x E x Unidad Datos.
#endif