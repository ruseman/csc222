CXX=g++
CPPFLAGS=
CXXFLAGS=-g -fPIC -Wall `sdl-config --cflags`
LDLIBS=`sdl-config --libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer -lstdc++
objects=main.o PlayerNode.o SceneDemo.o  Scene.o SceneNode.o MapNode.o Rectangle.o Map.o Input.o Graphics.o Image.o OutlineFont.o RasterFont.o Audio.o Game.o Music.o Sound.o

.PHONY: clean
all: main
main: $(objects)
%.o: %.cpp %.h
clean:
	rm -rf *.o main