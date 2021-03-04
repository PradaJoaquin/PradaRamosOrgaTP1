#ifndef __CACHEUTIL_H__
#define __CACHEUTIL_H__ 

typedef enum resultados
{
    hit,
    clean_miss,
    dirty_miss
}resultados_t;

typedef struct addr
{
	size_t tag;
	size_t index;
	size_t off;
}addr_t;

typedef struct op_result
{
    char operacion; 		// w o r
    resultados_t resultado;// Hit, clean miss o dirty miss.
    addr_t direccion;	  // tag, indice?
    size_t instruccion;  // podemos guardar aca la linea del archivo.
    bool valido; 		// Indica si se cargo un dato en la memoria o no. 
    bool dirty_bit;    // cambia el curso de algunas operaciones.
    size_t ant_tag;   // -1 por default, para modo verboso.
    size-t ant_bloque_ins; //anterior bloque->ins, para modo verboso.        
}op_result_t;

#endif