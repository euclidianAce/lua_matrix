CC=gcc
CFLAGS=-Wall -fPIC

BIN_DIR=bin
SRC_DIR=src
TARGET=$(BIN_DIR)/lua_matrix.so	# what to compile to
SRC=$(wildcard $(SRC_DIR)/*.c) 	# all the .c source files
OBJ=$(SRC:.c=.o)		# all the .o files, from the .c source files

%.o: %.c # compile .c files with -fPIC so they can be dynamically linked
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -shared -o $(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)

all: clean $(TARGET)
