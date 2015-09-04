#include <SDL/SDL.h>

//Create two surfaces
SDL_Surface* image = NULL;         //first surface
SDL_Surface* backbuffer = NULL;  //second surface

int main(int argc, char* args[])
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!");
        SDL_Quit();
        return 0;
    }

    //Create your window and return a pointer to the back buffer
    backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    //Put a caption in the title bar
    SDL_WM_SetCaption("SDL!!!", NULL);

    //Load the image
    image = SDL_LoadBMP("graphics/image.bmp" );

    if(image == NULL)
    {
        printf("Image failed to load!\n");
        SDL_Quit();
        return 0;
    }

    //Draw the image
    SDL_BlitSurface(image, NULL, backbuffer, NULL );
    SDL_Flip(backbuffer);

    //Wait
    SDL_Delay(3000);

    //Finish
    SDL_FreeSurface( image);
    SDL_Quit();

    return 1;
}

