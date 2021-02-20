#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>

#include "strutil.h"
#include "mensajes.h"

#define ESCRITURA "W"
#define LECTURA "R"

void procesar_comando(char** parametros) {
	char* operacion = parametros[1];
    char* direccion = parametros[2];
    int tamanio = atoi(parametros[3]);
    char* datos = parametros[4];
	
}


void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(FILE* archivo_de_trazas) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo_de_trazas) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		char** parametros = split(campos[1], ' ');
		procesar_comando(parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {
    // argv[1]: Archivo de traza
    // argv[2]: Tamaño en bytes
    // argv[3]: Asociatividad
    // argv[4]: Numero de sets

    // argv[5]: Modo verboso 
    // argv[6]: Inicio de rango
    // argv[7]: Fin de rango

	if(argc != 5 && argc != 8){
		printf(ERR_NUM_ARG);
		return 1;
	}

    FILE* archivo_trazas = fopen(argv[1], "r");
    if (!archivo_trazas){
        printf(ERR_ARCH_TRAZAS);
        return 1;
    }
	
    
    procesar_entrada(archivo_trazas);
    fclose(archivo_trazas);
	return 0;
}