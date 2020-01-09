CC=gcc
CFLAGS=-Wall -fPIC

SRC=$(wildcard src/*.c) 	# all the .c source files
OBJ=$(SRC:.c=.o)	# all the .o files, from the .c source files
BIN=bin
TARGET=$(BIN)/lua_matrix.so	# what to compile to

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -shared -o $(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)

all: clean $(TARGET)
