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
//cache_t* cache_crear(size_t tam, size_t asociatividad, size_t num_sets);
	size_t tam = 2048;
	size_t E = 2;
	size_t S = 64; 
	cache_t* cache = cache_crear(tam, E, S);
	printf("\n-----------------PRUEBAS DE CREACION/ DESTRUCCION--------------\n");
	printf(" Creo una cache vacia ");
	test(cache != NULL);

//void cache_destruir(cache_t* cache, size_t tope);
	cache_destruir(cache, S);
	return 0;
}