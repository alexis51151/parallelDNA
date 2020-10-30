BIN=apm apmMPI apmOMPMPI apmParallel 
SRC_DIR=src
HEADER_DIR=include
OBJ_DIR=obj

CC=gcc
MPICC=mpicc
CFLAGS=-g -O3 -I$(HEADER_DIR) -I/usr/include/mpi/ -fopenmp
LDFLAGS=

SRC= apm.c apmOMP.c apmMPI.c apmOMPMPI.c apmParallel.c

OBJ= $(OBJ_DIR)/apm.o $(OBJ_DIR)/apmOMP.o $(OBJ_DIR)/apmMPI.o $(OBJ_DIR)/apmOMPMPI.o $(OBJ_DIR)/apmParallel.o

all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

apm:$(OBJ_DIR)/apm.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

apmOMP:$(OBJ_DIR)/apmOMP.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

apmMPI:$(OBJ_DIR)/apmMPI.o
	$(MPICC) $(CFLAGS) $(LDFLAGS) -o $@ $^

apmOMPMPI:$(OBJ_DIR)/apmOMPMPI.o
	$(MPICC) $(CFLAGS) $(LDFLAGS) -o $@ $^

apmParallel:$(OBJ_DIR)/apmParallel.o
	$(MPICC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(BIN) $(OBJ)
