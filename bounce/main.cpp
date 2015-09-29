#include <SDL/SDL.h>

SDL_Surface* Background = NULL;
SDL_Surface* SpriteImage = NULL;
SDL_Surface* Backbuffer = NULL;

int SpriteFrame = 0;
int FrameCounter = 0;

const int MaxSpriteFrame = 11;
const int FrameDelay = 2;

int BackgroundX = 0;

#define INC 3

int freeX = 0;
int freeY = 250;
enum direction {
	NE, SE, NW, SW
} direction = NE;

SDL_Surface* LoadImage(char* fileName);
bool LoadFiles();
void FreeFiles();
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame);
bool ProgramIsRunning();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }

    Backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

    SDL_WM_SetCaption("Green is my pepper", NULL);

    if(!LoadFiles())
    {
        printf("Failed to load all files!\n");
        FreeFiles();
        SDL_Quit();

        return 0;
    }

    while(ProgramIsRunning())
    {
        //Update's the sprites frame
        FrameCounter++;

        if(FrameCounter > FrameDelay)
        {
            FrameCounter = 0;
            SpriteFrame++;
        }

        if(SpriteFrame > MaxSpriteFrame)
            SpriteFrame = 0;

        //Update's Background scrolling
        //BackgroundX-=6;
        //if(BackgroundX <= -800)
        //    BackgroundX = 0;

        //freeX += 3;
        //if(freeX >= 800)
        //	freeX = 0;

        // I ought to write a macro for this
        switch(direction){
        case NE:
        	// it's > N and E border
        	if(freeX >= 800-32 && freeY >= 600+32){
        		direction = SW;
        	}
        	// it's > E border
        	else if (freeX >= 800-32){
        		direction = NW;
        	}
        	// it's > N border
        	else if (freeY >= 600+32){
        		direction = SE;
        	}
        	// continue normally
        	else{
        		freeX+=INC;
        		freeY+=INC;
                //printf("NE\n");
        	}
        	break;
        case NW:
        	// it's greater than N and W border
        	if(freeX <= 0 && freeY >= 600+32){
        		direction = SE;
        	}
        	// it's > W border
        	else if (freeX <= 0){
        		direction = NE;
        	}
        	// it's > N border
        	else if (freeY >= 600 + 32){
        		direction = SW;
        	}
        	// continue normally
        	else{
        		freeX-=INC;
        		freeY+=INC;
                //printf("NW\n");
        	}
        	break;
        case SE:
        	// it's greater than S and E border
        	if(freeX >= 800-32 && freeY <= 0){
        		direction = NW;
        	}
        	// it's > E border
        	else if (freeX >= 800-32){
        		direction = SW;
        	}
        	// it's > S border
        	else if (freeY <= 0+32){
        		direction = NE;
        	}
        	else{
        		freeX+=INC;
        		freeY-=INC;
                //printf("SE\n");
        	}
        	break;
        case SW:
            // it's greater than S and W border
            if(freeX <= 0 && freeY <= 0){
                direction = NE;
            }
            // it's > W border
            else if (freeX <= 0){
                direction = SE;
            }
            // it's > S border
            else if (freeY <= 0+32){
                direction = NW;
            }
            // continue normally
            else{
                freeX-=INC;
                freeY-=INC;
                //printf("SW\n");
            }
            break;
        }



        //Render the scene
        DrawImage(Background,Backbuffer, BackgroundX, 0);
        DrawImage(Background,Backbuffer, BackgroundX+800, 0);
        DrawImage(SpriteImage, Backbuffer, freeX,600-freeY);

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
            Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0xFF, 0xFF);
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

    SpriteImage = LoadImage("graphics/free.bmp");

    if(SpriteImage == NULL)
        return false;

    return true;
}

void FreeFiles()
{
    SDL_FreeSurface(Background);
    SDL_FreeSurface(SpriteImage);
}

void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface(image, NULL, destSurface, &destRect);
}

void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int columns = image->w/width;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/columns)*height;
    sourceRect.x = (frame%columns)*width;
    sourceRect.w = width;
    sourceRect.h = height;

    SDL_BlitSurface(image, &sourceRect, destSurface, &destRect);
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

