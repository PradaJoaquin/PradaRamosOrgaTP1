#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "strutil.h"
#include "mensajes.h"
#include "simulador.h"

#define ESCRITURA "W"
#define LECTURA "R"

enum parametros{
	_,
	ruta,
	tam_cache,
	asociatividad,
	num_sets,
	modo_verboso,
	ini,
	fin
};

typedef struct argumentos{
	int tam_cache;
	int asociatividad;
	int  num_sets;
	char* modo_verboso;
	int ini;
	int fin;
}argumentos_t;

void procesar_comando(char** parametros, simulador_t* sim) {
	char* operacion = parametros[1];
    size_t direccion = strtoul(parametros[2], NULL, 16);
    int tamanio = atoi(parametros[3]);
    size_t datos = strtoul(parametros[4], NULL, 16);
	
	simulador_operar(sim, operacion, direccion, tamanio, datos);
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(FILE* archivo_de_trazas, simulador_t* sim) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo_de_trazas) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		char** parametros = split(campos[1], ' ');
		procesar_comando(parametros, sim);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

bool es_potencia_de_2(double numero) {
	return fmod(log2(numero), 1) == 0;
} 

bool argumentos_verificar(int argc, char** argv){
	if(argc != 5 && argc != 8){
		printf(ERR_NUM_ARG);
		return false;
	}
	if(!isdigit_strutil(argv[tam_cache]) || !isdigit_strutil(argv[asociatividad]) || !isdigit_strutil(argv[num_sets])){
		printf(ERR_NO_DIGIT_CACHE);
		return false;
	}
	if(!es_potencia_de_2(atof(argv[tam_cache])) || !es_potencia_de_2(atof(argv[asociatividad])) || !es_potencia_de_2(atof(argv[num_sets]))){
		printf(ERR_POT_2);
		return false;
	}
	if(argc == 8){
		if(strcmp("-v", argv[modo_verboso]) != 0){
			printf(ERR_PAR_MODO_VER);
			return false;
		}
		if(!isdigit_strutil(argv[ini]) || !isdigit_strutil(argv[fin])){
			printf(ERR_NO_DIGIT_MODO_VER);
			return false;
		}
		if(0 > atoi(argv[ini]) || atoi(argv[ini]) > atoi(argv[fin])){
			printf(ERR_NUM_MODO_VER);
			return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	if(!argumentos_verificar(argc, argv)){
		return 1;
	}
	argumentos_t args;
	args.tam_cache = atoi(argv[tam_cache]);
	args.asociatividad = atoi(argv[asociatividad]);
	args.num_sets = atoi(argv[num_sets]);
	args.modo_verboso = NULL;
	args.ini = 0;
	args.fin = 0;
	if(argc == 8){
		args.modo_verboso = argv[modo_verboso];
		args.ini = atoi(argv[ini]);
		args.fin = atoi(argv[fin]);
	}

    FILE* archivo_trazas = fopen(argv[ruta], "r");
    if (!archivo_trazas){
        printf(ERR_ARCH_TRAZAS);
        return 1;
    }

	simulador_t* sim = simulador_crear(args.tam_cache, args.asociatividad, args.num_sets, args.ini, args.fin);
    
    procesar_entrada(archivo_trazas, sim);
	simulador_destruir(sim);
    fclose(archivo_trazas);
	return 0;
}