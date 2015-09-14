#include <SDL/SDL.h>
#include <stdio.h>

SDL_Surface* Background = NULL;
SDL_Surface* SpriteImage = NULL;
SDL_Surface* ss1 = NULL;
SDL_Surface* ss2 = NULL;
SDL_Surface* Backbuffer = NULL;

SDL_Rect SpritePos;
SDL_Rect pos1;
SDL_Rect pos2;

SDL_Surface* LoadImage(char* fileName);
bool LoadFiles();
void FreeFiles();
bool ProgramIsRunning();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }

    Backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

    SDL_WM_SetCaption("Color Keying", NULL);

    if(!LoadFiles())
    {
        printf("Failed to load files!\n");
        FreeFiles();
        SDL_Quit();

        return 0;
    }

    pos1.x = 0;
    pos1.y = 250;

    pos2.x = 800;
    pos2.y = 250;

    while(ProgramIsRunning())
    {
    	pos1.x += 5;
    	pos2.x -= 5;

        if(pos1.x > 800)
            pos1.x = -200;

        if(pos2.x < 0)
        	pos2.x = 1000;

        SDL_BlitSurface(Background, NULL, Backbuffer, NULL);
        SDL_BlitSurface(ss1, NULL, Backbuffer, &pos1);
        SDL_BlitSurface(ss2, NULL, Backbuffer, &pos2);

        SDL_Delay(20);
        SDL_Flip(Backbuffer);
    }

    FreeFiles();

    SDL_Quit();

    return 0;
}

SDL_Surface* LoadImage(char* fileName)
{
    SDL_Surface* imageLoaded = NULL;
    SDL_Surface* processedImage = NULL;

    imageLoaded = SDL_LoadBMP(fileName);

    if(imageLoaded != NULL)
    {
        processedImage = SDL_DisplayFormat(imageLoaded);
        SDL_FreeSurface(imageLoaded);

        if(processedImage != NULL)
        {
            Uint32 colorKey = SDL_MapRGB(processedImage->format, 255, 0, 255);
            SDL_SetColorKey(processedImage, SDL_SRCCOLORKEY, colorKey);
        }

    }

    return processedImage;
}


bool LoadFiles()
{
    Background = LoadImage("graphics/background.bmp");

    if(Background == NULL)
        return false;

    SpriteImage = LoadImage("graphics/spaceship.bmp");


    if(SpriteImage == NULL)
        return false;

    ss1 = LoadImage("graphics/spaceship1.bmp");
    if(ss1 == NULL) return false;

    ss2 = LoadImage("graphics/spaceship2.bmp");
    if(ss2 == NULL) return false;

    return true;
}

void FreeFiles()
{
    SDL_FreeSurface(Background);
    SDL_FreeSurface(SpriteImage);
}

bool ProgramIsRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }

    return running;
}

