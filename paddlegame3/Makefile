CC=cc
CFLAGS= -Wall -ansi `sdl-config --cflags`
LFLAGS=-lSDL_mixer -lSDL_ttf -lSDL

all: bin/main
bin/main: bin object/main.o
	$(CC) -o bin/main object/main.o $(CFLAGS) `sdl-config --libs` $(LFLAGS)
object/main.o: object main.cpp
	$(CC) -c $(CFLAGS) main.cpp -o object/main.o
bin:
	mkdir -p bin
object:
	mkdir -p object
clean:
	rm -rf object
	rm -rf bin
run: bin/main
	bin/main
