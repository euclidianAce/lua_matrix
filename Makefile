CC=gcc
CFLAGS=-Wall -fPIC

SRC_DIR=src
TEST_DIR=testing

SRC=$(wildcard $(SRC_DIR)/*.c) 	# all the .c source files
OBJ=$(SRC:.c=.o)		# all the .o files, from the .c source files

TARGET=lmatrix.so
TESTTARGET=$(TEST_DIR)/runTests

LIBS=-llua -lm -ldl 

%.o: %.c # compile .c files with -fPIC so they can be dynamically linked
	$(CC) $(CFLAGS) -c $< -o $@

default: $(TARGET)

$(TARGET): $(filter-out $(SRC_DIR)/testing.o,$(OBJ))
	$(CC) $(filter-out $(SRC_DIR)/testing.o,$(OBJ)) -shared -o $(TARGET)

testing: $(TESTTARGET)

$(TESTTARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TESTTARGET) $(OBJ) $(LIBS)

clean: 
	rm -f $(OBJ) $(TARGET) \
	rm -f $(TESTTARGET)

all: clean default testing
