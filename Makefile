CC=gcc
CFLAGS = -g -Wall #add debug info and warnings

TARGET = lua_matrix


default: build

build:
	$(CC) -shared -o bin/$(TARGET).so -fPIC src/$(TARGET).c


clean:
	$(RM) bin/$(TARGET).so
