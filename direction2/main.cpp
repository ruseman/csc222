#include <SDL/SDL.h>

SDL_Surface* Background = NULL;
SDL_Surface* SpriteImage = NULL;
SDL_Surface* Backbuffer = NULL;

int SpriteX = 350;
int SpriteY = 250;

#define X_LOWER_BOUND 0
#define X_UPPER_BOUND 800 - 32

#define Y_LOWER_BOUND 0
#define Y_UPPER_BOUND 600 - 32


int dirX = 0;
int dirY = 0;

bool LoadFiles();
void FreeFiles();

SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
void moveShape();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    Backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

    SDL_WM_SetCaption("I was going to use a spaceship but I like richard stallman more", NULL);

    if(!LoadFiles())
    {
        FreeFiles();
        SDL_Quit();
        return 0;
    }

    while(ProgramIsRunning())
    {
        //Handle Input
        Uint8* keys = SDL_GetKeyState(NULL);

        if(keys[SDLK_ESCAPE])
            break;

        if(keys[SDLK_LEFT])
        	dirX = -1;

        if(keys[SDLK_RIGHT])
        	dirX = 1;

        if(keys[SDLK_UP])
            dirY = -1;

        if(keys[SDLK_DOWN])
            dirY = 1;

        if(keys[SDLK_BACKSPACE]){
        	dirX=0;
        	dirY=0;
        }

        moveShape();

        DrawImage(Background,Backbuffer, 0, 0);
        DrawImage(SpriteImage, Backbuffer, SpriteX, SpriteY);

        SDL_Delay(20);
        SDL_Flip(Backbuffer);
    }

    FreeFiles();

    SDL_Quit();

    return 1;
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

        if( processedImage != NULL )
        {
            Uint32 colorKey = SDL_MapRGB( processedImage->format, 0xFF, 0, 0xFF );
            SDL_SetColorKey( processedImage, SDL_SRCCOLORKEY, colorKey );
        }

    }

    return processedImage;
}

void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface( image, NULL, destSurface, &destRect);
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

bool LoadFiles()
{
    SpriteImage = LoadImage("graphics/free.bmp");

    if(SpriteImage == NULL)
        return false;

    Background = LoadImage("graphics/background.bmp");

    if(Background == NULL)
        return false;

    return true;

}

void FreeFiles()
{
    if(SpriteImage != NULL)
    {
        SDL_FreeSurface(SpriteImage);
        SpriteImage = NULL;
    }

    if(Background != NULL)
    {
        SDL_FreeSurface(Background);
        Background = NULL;
    }
}

void moveShape()
{
	if(dirX > 0){
		SpriteX += 4;
	}
	else if (dirX < 0){
		SpriteX -= 4;
	}

	if(dirY > 0){
		SpriteY+= 4;
	}else if (dirY < 0){
		SpriteY-= 4;
	}

	if(SpriteX > X_UPPER_BOUND)
		SpriteX = X_UPPER_BOUND;
	else if (SpriteX < X_LOWER_BOUND)
		SpriteX = X_LOWER_BOUND;

	if(SpriteY > Y_UPPER_BOUND)
		SpriteY = Y_UPPER_BOUND;
	else if (SpriteY < Y_LOWER_BOUND)
		SpriteY = Y_LOWER_BOUND;
}
