#include "cache.h"
#include "estadisticas.h"

#define COLOR_RED	   "\x1b[1m\x1b[31m"
#define COLOR_GREEN   "\x1b[1m\x1b[32m"
#define COLOR_BOLD_YELLOW "\x1b[01;33m"
#define COLOR_RESET   "\x1b[0m"

bool test (bool comparacion)
{   
    printf(" ...");
    if(comparacion) printf(COLOR_GREEN" OK\n");
    else printf(COLOR_RED " ERROR\n");    
    
    printf(COLOR_RESET );
    return comparacion;
}

int main()
{
	size_t tam = 2048;
	size_t E = 2;
	size_t S = 64; 
	size_t tamanio_bloque = tam/(E*S); 
	cache_t* cache = cache_crear(tam, E, S);
	printf("\n-----------------PRUEBAS DE CREACION/ DESTRUCCION--------------\n");
	
/*	
	//bloque_t* bloque = crear_bloques(E, tamanio_bloque); 
	printf(" Creo un vector de bloques ");
	test(bloque != NULL);
	printf(" Destruyo el bloque\n");
	destruir_bloques(bloque, E);
*/	
	printf(" Creo una cache vacia ");
	test(cache != NULL);

	printf(" Destruyo el bloque\n");
	cache_destruir(cache, S);
	return 0;
}