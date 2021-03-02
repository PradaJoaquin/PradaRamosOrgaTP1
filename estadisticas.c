#include "estadisticas.h"


size_t calcular_readtime(estadisticas_t* estadisticas){
	return estadisticas->lecturas +  ((estadisticas->rmiss + estadisticas->dirty_rmiss) * PENALTY);
}

size_t calcular_writetime(estadisticas_t* estadisticas){
	return estadisticas->escrituras + ((estadisticas->wmiss + estadisticas->dirty_wmiss) * PENALTY);
}

/*
* Tamaño del bloque debe estar validado. 
*/
size_t calcular_bytes_written(estadisticas_t* estadisticas, size_t tamanio_bloque){
	return ( (estadisticas->rmiss + estadisticas->wmiss) *  tamanio_bloque); 
}

size_t calcular_bytes_read(estadisticas_t* estadisticas, size_t tamanio_bloque){
	return ( (estadisticas->dirty_rmiss + estadisticas->dirty_wmiss) *  tamanio_bloque); 
}

float calcular_miss_rate(estadisticas_t* estadisticas)
{
	return (float)(estadisticas->rmiss + estadisticas->wmiss) / 
	(float)(estadisticas->escrituras + estadisticas->lecturas);
}


/*
* Recibe el tamanio_cache como cantidad de bytes.
*/
void imprimir_estadisticas(estadisticas_t* estadisticas, size_t sets, size_t E, size_t tamanio_cache)
{
	if(!estadisticas || sets == 0 || E == 0) return;
	size_t tamanio_bloque = tamanio_cache/ (sets*E);

	if(E == 1) printf("Direct-mapped, ");
	else printf("%ld-way, ", E);
	printf("%ld sets, size = %ldKB \n", sets tamanio_cache/KB);
	printf("Loads: %ld stores: %ld total: \n", estadisticas->lecturas, estadisticas->escrituras, estadisticas->lecturas + estadisticas->escrituras);
	printf("rmiss: %ld, wmiss: %ld total: \n", estadisticas->rmiss, estadisticas->wmiss, estadisticas->rmiss + estadisticas->wmiss);
	printf("dirty rmis: %ld dirty wmiss: %ld \n", estadisticas->dirty_rmiss, estadisticas->dirty_wmiss);
	printf("bytes read: %ld bytes written: %ld \n", calcular_bytes_read(estadisticas, tamanio_bloque), calcular_bytes_written(estadisticas, tamanio_bloque) );
	printf("Read time: %ld write time: %ld\n", calcular_readtime(estadisticas), calcular_writetime(estadisticas) );
	printf("Miss rate: %f \n", calcular_miss_rate(estadisticas) );
}