CXX=g++
CXXFLAGS=-g -Wall `sdl-config --cflags`
LDLIBS=`sdl-config --libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer -lstdc++
objects=main.o input.o graphics.o image.o outlineFont.o rasterFont.o audio.o game.o music.o sound.o

.PHONY: clean
.PHONY: objects
all: main
main: $(objects)
objects: $(objects)
clean:
	rm -rf *.o main