#include <SDL/SDL.h>

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!");
        SDL_Quit();
        return 0;
    }
    SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Hello World", NULL);
    SDL_SetVideoMode (800, 600, 32, SDL_FULLSCREEN | SDL_SWSURFACE);
    SDL_Delay(3000);
    SDL_Quit();

    return 1;
}

