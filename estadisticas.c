#include "estadisticas.h"

size_t penalizacion_rmisses(size_t rmiss){
	return rmiss * PENALTY;
}

size_t penalizacion_wmisses(size_t wmiss){
	return wmiss * PENALTY;
}

size_t penalizacion_dirty_rmisses(size_t dirty_rmiss){
	return dirty_rmiss * PENALTY;
}

size_t penalizacion_dirty_wmisses(size_t dirty_wmiss){
	return dirty_wmiss * PENALTY;
}

size_t calcular_readtime(estadisticas_t* estadisticas){
	return estadisticas->lecturas +  ((estadisticas->rmiss + estadisticas->dirty_rmiss) * PENALTY);
}

size_t calcular_writetime(estadisticas_t* estadisticas){
	return estadisticas->escrituras + ((estadisticas->wmiss + estadisticas->dirty_wmiss) * PENALTY);
}

//tam = 2048/(128) = 2 bytes por bloque --> 
size_t calcular_bytes_written(estadisticas_t* estadisticas, size_t tamanio_bloque){
	return (estadisticas->escrituras *  tamanio_bloque)/8; //mal
}

size_t calcular_bytes_read(estadisticas_t* estadisticas, size_t tamanio_bloque){
	return (estadisticas->escrituras *  tamanio_bloque)/8; //mal
}