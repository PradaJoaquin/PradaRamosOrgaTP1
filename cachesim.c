#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "strutil.h"
#include "mensajes.h"
#include "simulador.h"

/* 
	./cachesim adpcm.xex 2048 2 64 -v 0 15000
	2-way, 64 sets, size = 2KB		            //OK
	loads 65672 stores 34328 total 100000		//OK
	rmiss 515 wmiss 179 total 694				//ERROR
	dirty rmiss 158 dirty wmiss 11				//ERROR
	bytes read 11104 bytes written 2704			//ERROR
	read time 132972 write time 53328			//ERROR
	miss rate 0.006940

  ./cachesim adpcm.xex 4096 1 256 -v 0 10000
  	direct-mapped, 256 sets, size = 4KB
	loads 65672 stores 34328 total 100000
	rmiss 679 wmiss 419 total 1098
	dirty rmiss 197 dirty wmiss 390
	bytes read 17568 bytes written 9392
	read time 153272 write time 115228
	miss rate 0.010980
*/

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

// Se hace esto ya que el archivo de traza no esta separado constantemente por un espacio, sino que puede variar.
// Encuentra la siguiente posicion valida en el arreglo.
int sig_posision_valida(char** parametros, int actual){
	for(int i = actual + 1; parametros[i] != NULL; i++){
		if(strcmp(parametros[i], "") != 0){
			return i;
		}
	}
	return -1;
}

void procesar_comando(char** parametros, simulador_t* sim, estadisticas_t* estadisticas, size_t instruccion) {
	int pos_operacion = sig_posision_valida(parametros, 0);
	int pos_direccion = sig_posision_valida(parametros, pos_operacion);
	//int pos_tamanio = sig_posision_valida(parametros, pos_direccion);
	//int pos_datos = sig_posision_valida(parametros, pos_tamanio);

	char* operacion = parametros[pos_operacion];
    size_t direccion = strtoul(parametros[pos_direccion], NULL, 16);
    //int tamanio = atoi(parametros[pos_tamanio]);
    //size_t datos = strtoul(parametros[pos_datos], NULL, 16);
//	size_t instruccion = strtoul(inst, NULL, 16); 	//???, debe almacenar la linea del archivo 

//simulador_operar(simulador_t* sim, char* operacion, size_t direccion, size_t tam, size_t datos, estadisticas_t* estadisticas);
	simulador_operar(sim, operacion, direccion, instruccion, estadisticas);
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(FILE* archivo_de_trazas, simulador_t* sim, argumentos_t* args) {
	char* linea = NULL;
	size_t c = 0;
	estadisticas_t estadisticas;
	inicializar_estadisticas(&estadisticas);

	size_t instruccion = 0;
	while (getline(&linea, &c, archivo_de_trazas) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		char** parametros = split(campos[1], ' ');
		procesar_comando(parametros, sim, &estadisticas, instruccion);
		free_strv(parametros);
		free_strv(campos);	
		instruccion++;
	}
	free(linea);

	imprimir_estadisticas(&estadisticas, args->num_sets, args->asociatividad, args->tam_cache);
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
    
    procesar_entrada(archivo_trazas, sim, &args);
	simulador_destruir(sim);
    fclose(archivo_trazas);
	return 0;
}