CC = gcc
LANG_STD = -std=c99
COMPILER_FLAGS = -Wall
SRC_FILES = src/*.c
LINKER_FLAGS = -lm `pkg-config --cflags --libs raylib`
OBJ_NAME = arkanoid

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)

all: clean build
