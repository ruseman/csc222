#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

#define DEBUG
// comment out this line for debug information
#undef DEBUG

#define null NULL

// game constants
#define TITLE "GNU/Linux/X11/Paddles!"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define PADDLE_WIDTH (20)
#define PADDLE_HEIGHT (100)

#define BALL_WIDTH (20)
#define BALL_HEIGHT (20)

#define PLAYER_PADDLE_X (PADDLE_WIDTH)
#define TARGET_PADDLE_X (SCREEN_WIDTH - PADDLE_WIDTH * 2)

#define DIRECTION_MIN (0)
#define DIRECTION_MAX (360)

#define BALL_MAX_SPEED (20)
#define BALL_MIN_SPEED (5)
#define PLAYER_SPEED (10)

#define START_LIVES 3
#define MIN_LIVES 0

#define START_SCORE 0
#define WINNING 10

#define FPS (30)
#define FRAME_DELAY (1000/FPS)

#define BACKGROUND_IMAGE_PATH "assets/graphics/background.bmp"
#define BALL_IMAGE_PATH "assets/graphics/ball.bmp"
#define PLAYERPADDLE_IMAGE_PATH "assets/graphics/player.bmp"
#define TARGETPADDLE_IMAGE_PATH "assets/graphics/enemy.bmp"

#define FONT_PATH "assets/font.ttf"

#define BALLBOUNCE_SOUND_PATH "assets/sound/bounce.wav"
#define BALLSPAWN_SOUND_PATH "assets/sound/spawn.wav"
#define PLAYERSCORE_SOUND_PATH "assets/sound/playerscore.wav"
#define ENEMYSCORE_SOUND_PATH "assets/sound/enemyscore.wav"

#define MUSIC_PATH "assets/sound/song.mp3"

// surfaces
SDL_Surface* backbuffer = null;
SDL_Surface* backgroundImage = null;
SDL_Surface* ballImage = null;
SDL_Surface* playerPaddleImage = null;
SDL_Surface* targetPaddleImage = null;

// fonts
TTF_Font* font = null;

// sounds
Mix_Chunk* ballBounceSound = null;
Mix_Chunk* ballSpawnSound = null;
Mix_Chunk* playerScoreSound = null;
Mix_Chunk* enemyScoreSound = null;

// music
Mix_Music* music = null;

bool playerInit = false;
bool gameover = false;

// game vars
int playerScore = START_SCORE;
int lives = START_LIVES;

enum gamestate{
	RUNNING, WIN, LOSE
} state = RUNNING;

int ballX;
int ballY;

int ballYVel;
int ballXVel;

enum direction{
	NE, NW, SE, SW
};

SDL_Rect playerPaddleRect;
SDL_Rect targetPaddleRect;
SDL_Rect ballRect;

bool init();
bool initFiles();
bool initSDL();
bool isRunning();
int free();
SDL_Surface* loadImage(char*);
void freeFiles();
void run();
void reset();
void waitExit();
void exit();
void win();
void lose();
void draw();
void drawImage(SDL_Surface*, SDL_Surface*, int, int);
SDL_Surface* loadImage(char*, unsigned char, unsigned char, unsigned char);
SDL_Surface* loadImage(char*);
void drawText(SDL_Surface*, char*, int, int, TTF_Font*, Uint8, Uint8, Uint8);
bool rectsOverlap(SDL_Rect, SDL_Rect);
void updatePlayer();
void updateBall();

void updatePlayer(){
	Uint8 *keys = SDL_GetKeyState(null);

	if(keys[SDLK_UP])
		playerPaddleRect.y -= PLAYER_SPEED;
	if(keys[SDLK_DOWN])
		playerPaddleRect.y += PLAYER_SPEED;
#ifdef DEBUG
	if(keys[SDLK_SPACE])
		playerScore++;
#endif
	if(keys[SDLK_ESCAPE])
		exit();

	playerPaddleRect.y = playerPaddleRect.y < 0 ? 0 : playerPaddleRect.y;
	playerPaddleRect.y = playerPaddleRect.y > SCREEN_HEIGHT - playerPaddleRect.h ? SCREEN_HEIGHT - playerPaddleRect.h : playerPaddleRect.y;
}

int getRandVelocity(){
	int v = rand() % BALL_MAX_SPEED +1;
	return v < BALL_MIN_SPEED ? BALL_MIN_SPEED : v;
}

void updateBall(){
    ballRect.x += ballXVel;
    ballRect.y += ballYVel;

    //If the ball hits the player, make it bounce
    if(rectsOverlap(ballRect, playerPaddleRect))
    {
        ballXVel = getRandVelocity() * -1;
        Mix_PlayChannel(-1, ballBounceSound, 0);
    }

    //If the ball hits the target, make it bounce
    if(rectsOverlap(ballRect, targetPaddleRect))
    {
        ballXVel = getRandVelocity() * 1;
        Mix_PlayChannel(-1, playerScoreSound, 0);
        playerScore++;
    }

    //Make sure the ball doesn't leave the screen and make it
    //bounce randomly
    if(ballRect.y < 0)
    {
    	ballRect.y = 0;
        ballYVel = getRandVelocity();
        Mix_PlayChannel(-1, ballBounceSound, 0);
    }

    if(ballRect.y > SCREEN_HEIGHT - ballRect.h)
    {
        ballRect.y = SCREEN_HEIGHT - ballRect.h;
        ballYVel = getRandVelocity() * -1;
        Mix_PlayChannel(-1, ballBounceSound, 0);
    }

    if(ballRect.x < 0){
#ifdef DEBUG
    	printf("bounce left\n");
#endif
    	ballRect.x = 0 ;
    	ballXVel = getRandVelocity();
        Mix_PlayChannel(-1, ballBounceSound, 0);
    }

    //If player scores
    if(ballRect.x > SCREEN_WIDTH)
    {
    	lives--;
        Mix_PlayChannel(-1, enemyScoreSound, 0);
        reset();
    }
}

void win(){
#ifdef DEBUG
	printf("win\n");
#endif

	state = WIN;
}

void lose(){
#ifdef DEBUG
	printf("lose\n");
#endif
	state = LOSE;
}

void waitExit(){
	Uint8 *keys = SDL_GetKeyState(null);

	while(!keys[SDLK_ESCAPE]){
		SDL_Delay(10);
	}
	exit();
}

bool rectsOverlap(SDL_Rect r1, SDL_Rect r2){
    if(r1.x >= r2.x+r2.w)
        return false;
    if(r1.y >= r2.y+r2.h)
        return false;
    if(r2.x >= r1.x+r1.w)
        return false;
    if(r2.y >= r1.y+r1.h)
        return false;
    return true;
}

void drawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font,
		Uint8 r, Uint8 g, Uint8 b) {
	SDL_Surface* renderedText;
	SDL_Color color;

	color.r = r;
	color.g = g;
	color.b = b;

	renderedText = TTF_RenderText_Solid(font, string, color);

	SDL_Rect pos;

	pos.x = x;
	pos.y = y;

	SDL_BlitSurface(renderedText, null, surface, &pos);
	SDL_FreeSurface(renderedText);
}

void drawImage(SDL_Surface* image, SDL_Surface* dest, int x, int y) {
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	SDL_BlitSurface(image, null, dest, &destRect);
}

void exit(){
#ifdef DEBUG
	printf("Exiting gracefully\n");
#endif
	exit(0);
}

int main(int argc, char* argv[]) {
	if (!init())
		return free();
	printf("initialization completed successfully\n");

	while (isRunning()) {
		if (state==RUNNING){
#ifdef DEBUG
			printf("tick\n");
#endif
			long int oldtime = SDL_GetTicks();
			SDL_FillRect(backbuffer, null, 0);
			run();
			draw();

			int frametime = SDL_GetTicks() - oldtime;

			if (frametime < FRAME_DELAY)
				SDL_Delay(FRAME_DELAY - frametime);
			SDL_Flip(backbuffer);
		}
		else{
			draw();
			updatePlayer();
		}
	}
	return free();
}

bool isRunning() {
	SDL_Event event;

	bool running = true;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			running = false;
		if (event.type == SDL_KEYDOWN)
			if (event.key.keysym.sym == SDLK_ESCAPE)
				running = false;
	}

	return running;
}

void freeFiles() {
	// free images
	SDL_FreeSurface(backgroundImage);
	SDL_FreeSurface(ballImage);
	SDL_FreeSurface(playerPaddleImage);
	SDL_FreeSurface(targetPaddleImage);

	// free font
	TTF_CloseFont(font);

	// free sounds
	Mix_FreeChunk(ballBounceSound);
	Mix_FreeChunk(ballSpawnSound);
	Mix_FreeChunk(playerScoreSound);
	Mix_FreeChunk(enemyScoreSound);

	// free music
	Mix_FreeMusic(music);
}

bool init() {
#ifdef DEBUG
	printf("beginning initialization\n");
#endif
	// init SDL
	if (!initSDL())
		return false;
#ifdef DEBUG
	printf("\tSDL initialized\n");
#endif

	if (!initFiles())
		return false;
#ifdef DEBUG
	printf("\tfiles loaded\n");
#endif

	// set the title
	SDL_WM_SetCaption(TITLE, null);

	// set scores to 0
	playerScore = 0;

	// reset
	reset();

	// play music
	Mix_PlayMusic(music, -1);

	return true;
}

void run() {
	updatePlayer();
	updateBall();
	if(playerScore >= WINNING)
		win();
	if(lives <= 0)
		lose();
}


void reset() {
	if(!playerInit){
		playerPaddleRect.x = SCREEN_WIDTH - (PADDLE_WIDTH * 2);
		playerPaddleRect.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
		playerPaddleRect.w = PADDLE_WIDTH;
		playerPaddleRect.h = PADDLE_HEIGHT;
		playerInit = true;
	}

	targetPaddleRect.x = PADDLE_WIDTH * 2;
	targetPaddleRect.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
	targetPaddleRect.w = PADDLE_WIDTH;
	targetPaddleRect.h = PADDLE_HEIGHT;

	ballRect.x = 0;
	ballRect.y = rand()%SCREEN_HEIGHT+1;
	ballRect.w = BALL_WIDTH;
	ballRect.h = BALL_HEIGHT;

	ballXVel = getRandVelocity();
	ballYVel = getRandVelocity();

	if(ballXVel < 0)
		ballXVel *= -1;

	Mix_PlayChannel(-1, ballSpawnSound, 0);
}

void draw() {
	drawImage(backgroundImage, backbuffer, 0, 0);
	drawImage(ballImage, backbuffer, ballRect.x, ballRect.y);
	drawImage(playerPaddleImage, backbuffer, playerPaddleRect.x, playerPaddleRect.y);
	drawImage(targetPaddleImage, backbuffer, targetPaddleRect.x, targetPaddleRect.y);

	char playerHUD[64];
	char livesHUD[64];

	sprintf(playerHUD, "Player Score: %d", playerScore);
	sprintf(livesHUD, "Lives: %d", lives);

	drawText(backbuffer, playerHUD, 0, 1, font, 64, 64, 64);
	drawText(backbuffer, livesHUD, 0, 32, font, 64, 64, 64);

	if(state==WIN)
		drawText(backbuffer, "You win!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, font, 64, 64, 64);
	else if (state == LOSE)
		drawText(backbuffer, "You lose!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, font, 64, 64, 64);
}

int free() {
	Mix_HaltMusic();
	freeFiles();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	return 0;
}

bool initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}

	// init audio
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		return false;

	// init fonts
	if (TTF_Init() == -1)
		return false;

	backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			SDL_SWSURFACE);

	// error check backbuffer
	if (backbuffer == null)
		return false;

	return true;
}

bool initFiles() {
	backgroundImage = loadImage(BACKGROUND_IMAGE_PATH);
	ballImage = loadImage(BALL_IMAGE_PATH);
	playerPaddleImage = loadImage(PLAYERPADDLE_IMAGE_PATH);
	targetPaddleImage = loadImage(TARGETPADDLE_IMAGE_PATH);

	if (backgroundImage == null)
		return false;
	if (ballImage == null)
		return false;
	if (playerPaddleImage == null)
		return false;
	if (targetPaddleImage == null)
		return false;

	font = TTF_OpenFont(FONT_PATH, 30);

	if (font == null)
		return false;

	ballBounceSound = Mix_LoadWAV(BALLBOUNCE_SOUND_PATH);
	ballSpawnSound = Mix_LoadWAV(BALLSPAWN_SOUND_PATH);
	playerScoreSound = Mix_LoadWAV(PLAYERSCORE_SOUND_PATH);
	enemyScoreSound = Mix_LoadWAV(ENEMYSCORE_SOUND_PATH);

	if (ballBounceSound == null) {
		return false;
	}
	if (ballSpawnSound == null)
		return false;
	if (playerScoreSound == null)
		return false;
	if (enemyScoreSound == null)
		return false;

	music = Mix_LoadMUS(MUSIC_PATH);

	if (music == null)
		return false;

	return true;
}

SDL_Surface* loadImage(char* filename, unsigned char r, unsigned char g,
		unsigned char b) {
	SDL_Surface* imageLoaded = null;
	SDL_Surface* processedImage = null;
	imageLoaded = SDL_LoadBMP(filename);

	if (imageLoaded != null) {
		processedImage = SDL_DisplayFormat(imageLoaded);
		SDL_FreeSurface(imageLoaded);

		if (processedImage != null) {
			Uint32 colorkey = SDL_MapRGB(processedImage->format, r, g, b);
			SDL_SetColorKey(processedImage, SDL_SRCCOLORKEY, colorkey);
		}
	}
	return processedImage;
}

SDL_Surface* loadImage(char* filename) {
	return loadImage(filename, 0xFF, 0, 0xFF);
}
