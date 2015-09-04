#include <SDL/SDL.h>

SDL_Surface* background = NULL;
SDL_Surface* sprite = NULL;
SDL_Surface* backbuffer = NULL;

bool ProgramIsRunning();
bool LoadImages();
void FreeImages();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!\n");
        SDL_Quit();
        return 0;
    }

    backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Timothy Miller", NULL);
    if(!LoadImages())
    {
        printf("Images failed to load!\n");
        FreeImages();
        SDL_Quit();
        return 0;
    }
    SDL_BlitSurface(background, NULL, backbuffer, NULL ); //blit the background to the buffer

    //Here is the Game Loop!!!
    while(ProgramIsRunning())
    {
        SDL_Rect spritePos;           //define a structure (SDL_Rect) that contains data for a rectangle (spritePos).
        spritePos.x = rand()%800;   //select an x coordinate between 0 and 800
        spritePos.y = rand()%600;   //select a y coordinate between 0 and 600

        SDL_BlitSurface(sprite, NULL, backbuffer, &spritePos);  //blit the rectangle to the buffer
        SDL_Flip(backbuffer);                                                  //flip the buffer and draw to the game screen
        SDL_Delay(100);                                                        // wait 1/10th of a second before drawing the next sprite to a random location
    }

    SDL_Quit();
    return 1;
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

bool LoadImages()
{
    background = SDL_LoadBMP("graphics/background.bmp");
    if(background == NULL)
        return false;
    sprite = SDL_LoadBMP("graphics/sprite.bmp");

    if(sprite == NULL)
        return false;
    return true;
}

void FreeImages()
{
    if(background != NULL)
    {
        SDL_FreeSurface(background);
        background = NULL;
    }

    if(sprite != NULL)
    {
        SDL_FreeSurface(sprite);
        sprite = NULL;
    }
}

