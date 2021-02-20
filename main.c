#include <stdlib.h>
#include <stdio.h>

#include "mensajes.h"



int main(int argc, char** argv) {
	if(argc != 5 && argc != 8){
		printf(ERR_NUM_ARG);
		return 1;
	}
	
	return 0;
}