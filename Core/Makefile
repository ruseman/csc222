CXX=g++
CPPFLAGS=
CXXFLAGS=-g -fPIC -Wall `sdl-config --cflags`
LDLIBS=`sdl-config --libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer -lstdc++
objects=Input.o Graphics.o Image.o OutlineFont.o RasterFont.o Audio.o Game.o Music.o Sound.o

.PHONY: clean
all: libcore.so
libcore.so: $(objects)
	$(CXX) $(LDFLAGS) $(objects) -shared -g -o libcore.so
clean:
	rm -rf *.o libcore.so