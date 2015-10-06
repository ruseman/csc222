#include <SDL/SDL.h>

SDL_Surface* spriteBackground = NULL;
SDL_Surface* spriteImage = NULL;
SDL_Surface* backbuffer = NULL;

int SpriteX = 350;
int SpriteY = 250;

#define X_LOWER_BOUND 0
#define X_UPPER_BOUND 800 - 32

#define Y_LOWER_BOUND 0
#define Y_UPPER_BOUND 600 - 32


int dirX = 0;
int dirY = 0;

bool loadFiles();
void freeFiles();

SDL_Surface* loadImage(char* fileName);
void drawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool programIsRunning();
void moveShape();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

    SDL_WM_SetCaption("I was going to use a spaceship but I like richard stallman more", NULL);

    if(!loadFiles())
    {
        freeFiles();
        SDL_Quit();
        return 0;
    }

    while(programIsRunning())
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

        drawImage(spriteBackground,backbuffer, 0, 0);
        drawImage(spriteImage, backbuffer, SpriteX, SpriteY);

        SDL_Delay(20);
        SDL_Flip(backbuffer);
    }

    freeFiles();

    SDL_Quit();

    return 1;
}

SDL_Surface* loadImage(char* fileName)
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

void drawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface( image, NULL, destSurface, &destRect);
}

bool programIsRunning()
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

bool loadFiles()
{
    spriteImage = loadImage("graphics/free.bmp");

    if(spriteImage == NULL)
        return false;

    spriteBackground = loadImage("graphics/background.bmp");

    if(spriteBackground == NULL)
        return false;

    return true;

}

void freeFiles()
{
    if(spriteImage != NULL)
    {
        SDL_FreeSurface(spriteImage);
        spriteImage = NULL;
    }

    if(spriteBackground != NULL)
    {
        SDL_FreeSurface(spriteBackground);
        spriteBackground = NULL;
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
