#ifndef MUSIC_H
#define MUSIC_H

#include "audio.h"

class Music
{
private:
    Mix_Music* music;
public:
    Music();
    ~Music();

    bool load(char fileName[]);
    void free();

    void play(int loops = 0);
    bool isLoaded();
};

#endif