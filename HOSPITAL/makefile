# Projeto SO - Fase 1
# Grupo 10
# Daniela Camarinha 58199
# Diogo Lopes 60447
# Pedro Silva 59886

OBJ_dir = obj
SRC_dir = src
BIN_dir = bin
INC_dir = include

EXEC = hOSpital

OBJECTS = main.o memory.o process.o patient.o receptionist.o doctor.o configuration.o hospsignal.o hosptime.o log.o stats.o synchronization.o

CC = gcc
CFLAGS = -Wall -I $(INC_DIR)
LIBS = -lrt

vpath %.o $(OBJ_dir)

out: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o $(BIN_dir)/$(EXEC) $(LIBS) -g

%.o: $(SRC_dir)/%.c
	$(CC) -I $(INC_dir) -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f $(OBJ_dir)/*.o
	rm -f $(BIN_dir)/$(EXEC)
	@echo "Apagando arquivos objetos e executáveis..."