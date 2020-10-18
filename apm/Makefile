BIN=apm apmOMP
SRC_DIR=src
HEADER_DIR=include
OBJ_DIR=obj

CC=gcc
CFLAGS=-O3 -I$(HEADER_DIR)
LDFLAGS=

SRC= apm.c apmOMP.c

OBJ= $(OBJ_DIR)/apm.o $(OBJ_DIR)/apmOMP.o

all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

apm:$(OBJ_DIR)/apm.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	
apmOMP:$(OBJ_DIR)/apmOMP.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


clean:
	rm -f $(BIN) $(OBJ)
