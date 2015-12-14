#include "music.h"

Music::Music()
{
    music = NULL;
}

Music::~Music()
{

}

bool Music::load(char fileName[])
{
    music = Mix_LoadMUS(fileName);

    if(music == NULL)
    {
        printf("Failed to load song: %s\n", fileName);

        return false;
    }

    return true;
}

void Music::free()
{
    if(music != NULL)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
}

void Music::play(int loops)
{
    if(music != NULL)
        Mix_PlayMusic(music, loops);
}

bool Music::isLoaded()
{
    return (music != NULL);
}