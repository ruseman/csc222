#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define DEBUG 1

// various macros
#define null NULL

// file paths
#define	SPRITEPATH_BACKGROUND	"assets/graphics/background.bmp"
#define	SPRITEPATH_STALLMAN		"assets/graphics/free.bmp"
#define SPRITEPATH_DOOT			"assets/graphics/doot.bmp"
#define SOUNDPATH_HORN			"assets/sound/doot.wav"
#define SOUNDPATH_KEK			"assets/sound/kek.wav"
#define SOUNDPATH_FREEDOM		"assets/sound/freedom.wav"

// window settings
#define HEIGHT 600
#define WIDTH 800
#define CAPTION "Install Gentoo"

// fn declarations
int main(int argc, char* args[]);
SDL_Surface* loadImage(char* path);
void drawImage(SDL_Surface* image, SDL_Surface* dest, int x, int y);
bool isRunning();
bool loadFiles();
void freeFiles();
int exit();
bool check(int* x, int* y, int maxX, int maxY, int minX, int minY);
int playSound(Mix_Chunk* sound);

// sprites
SDL_Surface*	background	=	null;
SDL_Surface*	stallman	=	null;
SDL_Surface*	doot		=	null;
SDL_Surface*	backbuffer	=	null;

// sounds
Mix_Chunk*		horn		=	null;
Mix_Chunk*		kek			=	null;
Mix_Music*		freedom		=	null;

// constants
#define DOOT_DIM 64
#define STALLMAN_DIM 32
#define DOOT_Y_MAX HEIGHT
#define DOOT_X_MAX (WIDTH - DOOT_DIM)
#define DOOT_X_MIN (0)
#define DOOT_Y_MIN DOOT_DIM
#define DOOT_SPEED (8)
#define STALLMAN_Y_MAX HEIGHT
#define STALLMAN_X_MAX (WIDTH - STALLMAN_DIM)
#define STALLMAN_X_MIN (0)
#define STALLMAN_Y_MIN STALLMAN_DIM
#define STALLMAN_SPEED (4)

// position of stallman
int stallmanX	=	(2 * (STALLMAN_X_MAX / 3));
int stallmanY	=	(STALLMAN_Y_MAX / 2);

// position of doot
int dootX		=	(DOOT_X_MAX / 3);
int dootY		=	(DOOT_Y_MAX / 2);

// song
bool songPlaying = true;

int main(int argc, char* args[]){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return exit();
#ifdef DEBUG
	printf("SDL Initialized\n");
#endif

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
#ifdef DEBUG
	printf("SDL Mixer Initialized\n");
#endif

	backbuffer = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);
#ifdef DEBUG
	printf("Initialized video mode\n");
#endif

	SDL_WM_SetCaption(CAPTION, null);
#ifdef DEBUG
	printf("Set caption\n");
#endif

	if(!loadFiles())
		return exit();
#ifdef DEBUG
	printf("Files loaded\n");
#endif

	Mix_PlayMusic(freedom, -1);
#ifdef DEBUG
	printf("Music is playing\n");
#endif

	while(isRunning()){
		// main loop
		Uint8* keys = SDL_GetKeyState(null);

		if(keys[SDLK_ESCAPE])
			break;
		if(keys[SDLK_LEFT])
			dootX -= DOOT_SPEED;
		if(keys[SDLK_RIGHT])
			dootX += DOOT_SPEED;
		if(keys[SDLK_UP])
			dootY += DOOT_SPEED;
		if(keys[SDLK_DOWN])
			dootY -= DOOT_SPEED;
		if(keys[SDLK_w])
			stallmanY += STALLMAN_SPEED;
		if(keys[SDLK_s])
			stallmanY -= STALLMAN_SPEED;
		if(keys[SDLK_a])
			stallmanX -= STALLMAN_SPEED;
		if(keys[SDLK_d])
			stallmanX += STALLMAN_SPEED;
		if(keys[SDLK_SPACE])
			Mix_PauseMusic();
		if(keys[SDLK_TAB])
			Mix_ResumeMusic();

		bool dootBounce = check(&dootX, &dootY, DOOT_X_MAX, DOOT_Y_MAX, DOOT_X_MIN, DOOT_Y_MIN);
		bool stallmanBounce = check(&stallmanX, &stallmanY, STALLMAN_X_MAX, STALLMAN_Y_MAX, STALLMAN_X_MIN, STALLMAN_Y_MIN);

		// play sounds
		if(dootBounce)
			playSound(horn);
		if(stallmanBounce)
			playSound(kek);

		// draw background
		drawImage(background, backbuffer, 0, 0);

		// draw sprites
		drawImage(doot, backbuffer, (dootX), (HEIGHT - dootY));
		drawImage(stallman, backbuffer, (stallmanX), (HEIGHT - stallmanY));

		// draw buffer
		SDL_Delay(20);
		SDL_Flip(backbuffer);
	}
	exit();
}

int exit(){
	freeFiles();
	Mix_HaltMusic();
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}


bool isRunning(){
	SDL_Event event;
	bool running = true;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			running = false;
	}
	return running;
}

bool loadFiles(){
	background = loadImage(SPRITEPATH_BACKGROUND);
	if(background==null)
		return false;
	stallman = loadImage(SPRITEPATH_STALLMAN);
	if(stallman==null)
		return false;
	doot = loadImage(SPRITEPATH_DOOT);
	if(doot==null)
		return false;
	kek = Mix_LoadWAV(SOUNDPATH_KEK);
	if(kek == null)
		return false;
	horn = Mix_LoadWAV(SOUNDPATH_HORN);
	if(horn == null)
		return false;
	freedom = Mix_LoadMUS(SOUNDPATH_FREEDOM);
	if(freedom == null)
		return false;
	return true;
}

void freeFiles(){
	if(background!=null){
		SDL_FreeSurface(background);
		background= null;
	}
	if(stallman!=null){
		SDL_FreeSurface(stallman);
		stallman = null;
	}
	if(doot != null){
		SDL_FreeSurface(doot);
		doot = null;
	}
	if(kek != null){
		Mix_FreeChunk(kek);
		kek = null;
	}
	if(horn != null){
		Mix_FreeChunk(horn);
		horn = null;
	}
	if(freedom != null){
		Mix_FreeMusic(freedom);
		freedom = null;
	}
}

int playSound(Mix_Chunk* sound){
	return Mix_PlayChannel(-1, sound, 0);
}

void drawImage(SDL_Surface* image, SDL_Surface* dest, int x, int y){
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	SDL_BlitSurface(image, null, dest, &destRect);
}

SDL_Surface* loadImage(char* path){
	SDL_Surface* imageLoaded = null;
	SDL_Surface* processedImage = NULL;

	imageLoaded = SDL_LoadBMP(path);

	if(imageLoaded != null){
		processedImage = SDL_DisplayFormat(imageLoaded);
		SDL_FreeSurface(imageLoaded);

		if(processedImage != NULL){
			Uint32 colorKey = SDL_MapRGB(processedImage->format, 00, 00, 00);
			SDL_SetColorKey(processedImage, SDL_SRCCOLORKEY, colorKey);
		}
	}
	return processedImage;
}

bool check(int* x, int* y, int maxX, int maxY, int minX, int minY){
	int oldX = *x;
	int oldY = *y;
	*x = *x <= minX ? minX : *x;
	*x = *x >= maxX ? maxX : *x;
	*y = *y <= minY ? minY : *y;
	*y = *y >= maxY ? maxY : *y;
	return (oldY != *y) || (oldX != *x);
}
