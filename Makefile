#COMP ?= cc 
OBJS =  $(patsubst %.c,%.o,$(wildcard *.c)) # Todos los .o en carpeta
HEADERS = $(patsubst %.c,%.h,$(wildcard *.h)) # Todos los .h en carpeta, pero deben tener el mismo nombre que los .c
EXE = cachesim
#CFLAGS ?= -g -O2 -std=gnu11 -Wall -Wextra -Wvla
CFLAGS ?= -g -DCORRECTOR -O2 -std=gnu11 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion -Wextra -Wvla
LDLIBS := -lm
#$(patsubst %.c,%.h,$(wildcard *.c)) # Todos los .h en carpeta, pero deben tener el mismo nombre que los .c
# Si escribo make se ejecuta las reglas que indique a continuacion
# Correr el EXE
all: cachesim
run: 	$(EXE)
	./$(EXE)
# Compila el programa principal
cachesim: $(OBJS) $(HEADERS) 
#$(COMP) $(CFLAGS) -o $(EXE) $(OBJS) $(HEADERS)

# Genera los archivos.o individuales
%.o: %.c $(HEADERS)
# Elimina los archivos.o
clean:
	rm -f $(OBJS)
# Elimina el archivo EXE
clean_exe:
	rm -f $(EXE)
# Elimina los arhivos o y el EXE
clean_all: clean clean_exe
.PHONY: clean
