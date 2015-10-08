#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define null NULL

//Game constants
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT 600

#define PADDLE_WIDTH (20)
#define PADDLE_HEIGHT (100)
#define BALL_WIDTH (20)
#define BALL_HEIGHT (20)
#define BALL_MAX_SPEED (20)

#define PLAYER_PADDLE_X (PADDLE_WIDTH)
#define ENEMY_PADDLE_X (SCREEN_WIDTH - PADDLE_WIDTH*2)

#define PLAYER_SPEED (10)
#define ENEMY_SPEED (7)

#define FPS (30)
#define FRAME_DELAY (1000/FPS)

//Surfaces
SDL_Surface *Backbuffer = null;
SDL_Surface *BackgroundImage = null;
SDL_Surface *BallImage = null;
SDL_Surface *PlayerPaddleImage = null;
SDL_Surface *EnemyPaddleImage = null;

//Font
TTF_Font *GameFont = null;

//Sounds
Mix_Chunk *BallBounceSound = null;
Mix_Chunk *BallSpawnSound = null;
Mix_Chunk *PlayerScoreSound = null;
Mix_Chunk *EnemyScoreSound = null;

//Music
Mix_Music *GameMusic = null;

//Game Variables
int PlayerScore;
int EnemyScore;

int BallXVel;
int BallYVel;

SDL_Rect PlayerPaddleRect;
SDL_Rect EnemyPaddleRect;
SDL_Rect BallRect;

SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
bool ProgramIsRunning();
bool LoadFiles();
void FreeFiles();
bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);
bool InitSDL();
void ResetGame();
bool InitGame();
void UpdatePlayer();
void UpdateAI();
void UpdateBall();
void RunGame();
void DrawGame();
void FreeGame();

int main(int argc, char *argv[])
{
    if(!InitGame())
    {
        FreeGame();   //If InitGame failed, kill the program
        return 0;
    }

    while(ProgramIsRunning())
    {
        long int oldTime = SDL_GetTicks();  //We will use this later to see how long it took to update the frame
        SDL_FillRect(Backbuffer, null, 0);  //Clear the screen
        RunGame();                          //Update the game
        DrawGame();                         //Draw the screen

        int frameTime = SDL_GetTicks() - oldTime;

        if(frameTime < FRAME_DELAY)            //Dont delay if we dont need to
            SDL_Delay(FRAME_DELAY - frameTime);     //Delay
        SDL_Flip(Backbuffer);               //Flip the screen
    }

    FreeGame();     //Gracefully release SDL and its resources.

    return 0;
}

SDL_Surface* LoadImage(char* fileName)
{
    SDL_Surface* imageLoaded = null;
    SDL_Surface* processedImage = null;

    imageLoaded = SDL_LoadBMP(fileName);

    if(imageLoaded != null)
    {
        processedImage = SDL_DisplayFormat(imageLoaded);
        SDL_FreeSurface(imageLoaded);

        if( processedImage != null )
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

    SDL_BlitSurface( image, null, destSurface, &destRect);
}

void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface* renderedText = null;

    SDL_Color color;

    color.r = r;
    color.g = g;
    color.b = b;

    renderedText = TTF_RenderText_Solid( font, string, color );

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

    SDL_BlitSurface( renderedText, null, surface, &pos );
    SDL_FreeSurface(renderedText);
}

bool ProgramIsRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;

        if( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                    running = false;
            }
        }
    }

    return running;
}

bool LoadFiles()
{
    //Load images
    BackgroundImage = LoadImage("graphics/background.bmp");
    BallImage = LoadImage("graphics/ball.bmp");
    PlayerPaddleImage = LoadImage("graphics/player.bmp");
    EnemyPaddleImage = LoadImage("graphics/enemy.bmp");

    //Error checking images
    if(BackgroundImage == null)
        return false;
    if(BallImage == null)
        return false;
    if(PlayerPaddleImage == null)
        return false;
    if(EnemyPaddleImage == null)
        return false;

    //Load font
    GameFont = TTF_OpenFont("graphics/alfphabet.ttf", 30);

    //Error check font
    if(GameFont == null)
        return false;

    //Load sounds
    BallBounceSound = Mix_LoadWAV("audio/ballBounce.wav");
    BallSpawnSound = Mix_LoadWAV("audio/ballSpawn.wav");
    PlayerScoreSound = Mix_LoadWAV("audio/playerScore.wav");
    EnemyScoreSound = Mix_LoadWAV("audio/enemyScore.wav");

    //Error check sounds
    if(BallBounceSound == null)
        return false;
    if(BallSpawnSound == null)
        return false;
    if(PlayerScoreSound == null)
        return false;
    if(EnemyScoreSound == null)
        return false;

    //Load music
    GameMusic = Mix_LoadMUS("audio/song.mp3");

    //Error check music
    if(GameMusic == null)
        return false;

    return true;
}

void FreeFiles()
{
    //Free images
    SDL_FreeSurface(BackgroundImage);
    SDL_FreeSurface(BallImage);
    SDL_FreeSurface(PlayerPaddleImage);
    SDL_FreeSurface(EnemyPaddleImage);

    //Free font
    TTF_CloseFont(GameFont);

    //Free sounds
    Mix_FreeChunk(BallBounceSound);
    Mix_FreeChunk(BallSpawnSound);
    Mix_FreeChunk(PlayerScoreSound);
    Mix_FreeChunk(EnemyScoreSound);

    //Free music
    Mix_FreeMusic(GameMusic);
}

bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
    if(rect1.x >= rect2.x+rect2.w)
        return false;

    if(rect1.y >= rect2.y+rect2.h)
        return false;

    if(rect2.x >= rect1.x+rect1.w)
        return false;

    if(rect2.y >= rect1.y+rect1.h)
        return false;

    return true;
}

bool InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    //Init audio subsystem
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        return false;
    }

    //Init TTF subsystem
    if(TTF_Init() == -1)
    {
        return false;
    }

    //Generate screen
    Backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE );

    //Error check Backbuffer
    if(Backbuffer == null)
        return false;

    return true;
}

void ResetGame()
{
    //Position the player's paddle
    PlayerPaddleRect.x = PLAYER_PADDLE_X;
    PlayerPaddleRect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    PlayerPaddleRect.w = PADDLE_WIDTH;
    PlayerPaddleRect.h = PADDLE_HEIGHT;

    //Position the enemie's paddle
    EnemyPaddleRect.x = ENEMY_PADDLE_X;
    EnemyPaddleRect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    EnemyPaddleRect.w = PADDLE_WIDTH;
    EnemyPaddleRect.h = PADDLE_HEIGHT;

    //Position the ball
    BallRect.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
    BallRect.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
    BallRect.w = BALL_WIDTH;
    BallRect.h = BALL_HEIGHT;

    //Make the ball X velocity a random value from 1 to BALL_MAX_SPEED
    BallXVel = rand()%BALL_MAX_SPEED + 1;

    //Make the ball Y velocity a random value from -BALL_MAX_SPEED to BALL_MAX_SPEED
    BallYVel = (rand()%BALL_MAX_SPEED*2 + 1) - BALL_MAX_SPEED;

    //Give it a 50% probability of going toward's the player
    if(rand()%2 == 0)
        BallXVel *= -1;


    //Play the spawn sound
    Mix_PlayChannel(-1, BallSpawnSound, 0);
}

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Load Files
    if(!LoadFiles())
        return false;

    //Initiatialize game variables

    //Set the title
    SDL_WM_SetCaption("Paddle Game!",null);

    //Set scores to 0
    PlayerScore = 0;
    EnemyScore = 0;

    //This can also set the initial variables
    ResetGame();

    //Play Music
    Mix_PlayMusic(GameMusic, -1);

    return true;
}

void UpdatePlayer()
{
    Uint8 *keys = SDL_GetKeyState(null);

    //Move the paddle when the up/down key is pressed
    if(keys[SDLK_UP])
        PlayerPaddleRect.y -= PLAYER_SPEED;

    if(keys[SDLK_DOWN])
        PlayerPaddleRect.y += PLAYER_SPEED;

    //Make sure the paddle doesn't leave the screen
    if(PlayerPaddleRect.y < 0)
        PlayerPaddleRect.y = 0;

    if(PlayerPaddleRect.y > SCREEN_HEIGHT-PlayerPaddleRect.h)
        PlayerPaddleRect.y = SCREEN_HEIGHT-PlayerPaddleRect.h;
}

void UpdateAI()
{
//    //If the paddle's center higher than the ball's center, move the paddle up
//    if((EnemyPaddleRect.y + EnemyPaddleRect.h/2) > (BallRect.y+BallRect.h/2))
//        EnemyPaddleRect.y -= ENEMY_SPEED;
//
//    //If the paddle's center lower than the ball's center, move the paddle down
//    if((EnemyPaddleRect.y + EnemyPaddleRect.h/2) < (BallRect.y+BallRect.h/2))
//        EnemyPaddleRect.y += ENEMY_SPEED;
//
//    //Make sure the paddle doesn't leave the screen
//    if(EnemyPaddleRect.y < 0)
//        EnemyPaddleRect.y = 0;
//
//    if(EnemyPaddleRect.y > SCREEN_HEIGHT-EnemyPaddleRect.h)
//        EnemyPaddleRect.y = SCREEN_HEIGHT-EnemyPaddleRect.h;

    Uint8 *keys = SDL_GetKeyState(null);

    // move the paddle when the w/s key is pressed
    if(keys[SDLK_w])
    	EnemyPaddleRect.y -= ENEMY_SPEED;
    if(keys[SDLK_s])
    	EnemyPaddleRect.y += ENEMY_SPEED;

    // make sure the paddle doesn't leave the screen
    if(EnemyPaddleRect.y < 0)
    	EnemyPaddleRect.y = 0;
    if(EnemyPaddleRect.y > SCREEN_HEIGHT - EnemyPaddleRect.h)
    	EnemyPaddleRect.y = SCREEN_HEIGHT - EnemyPaddleRect.h;
}

void UpdateBall()
{
    BallRect.x += BallXVel;
    BallRect.y += BallYVel;

    //If the ball hits the player, make it bounce
    if(RectsOverlap(BallRect, PlayerPaddleRect))
    {
        BallXVel = rand()%BALL_MAX_SPEED + 1;
        Mix_PlayChannel(-1, BallBounceSound, 0);
    }

    //If the ball hits the enemy, make it bounce
    if(RectsOverlap(BallRect, EnemyPaddleRect))
    {
        BallXVel = (rand()%BALL_MAX_SPEED +1) * -1;
        Mix_PlayChannel(-1, BallBounceSound, 0);
    }

    //Make sure the ball doesn't leave the screen and make it
    //bounce randomly
    if(BallRect.y < 0)
    {
        BallRect.y = 0;
        BallYVel = rand()%BALL_MAX_SPEED + 1;
        Mix_PlayChannel(-1, BallBounceSound, 0);
    }

    if(BallRect.y > SCREEN_HEIGHT - BallRect.h)
    {
        BallRect.y = SCREEN_HEIGHT - BallRect.h;
        BallYVel = (rand()%BALL_MAX_SPEED + 1)* -1;
        Mix_PlayChannel(-1, BallBounceSound, 0);
    }

    //If player scores
    if(BallRect.x > SCREEN_WIDTH)
    {
        PlayerScore++;
        Mix_PlayChannel(-1, PlayerScoreSound, 0);
        ResetGame();
    }

    //If enemy scores
    if(BallRect.x < 0-BallRect.h)
    {
        EnemyScore++;
        Mix_PlayChannel(-1, EnemyScoreSound, 0);
        ResetGame();
    }
}

void RunGame()
{
    UpdatePlayer();
    UpdateAI();
    UpdateBall();
}

void DrawGame()
{
    DrawImage(BackgroundImage, Backbuffer, 0, 0);
    DrawImage(BallImage, Backbuffer, BallRect.x, BallRect.y);
    DrawImage(PlayerPaddleImage, Backbuffer, PlayerPaddleRect.x, PlayerPaddleRect.y);
    DrawImage(EnemyPaddleImage, Backbuffer, EnemyPaddleRect.x, EnemyPaddleRect.y);

    char playerHUD[64];
    char enemyHUD[64];

    sprintf(playerHUD, "Player Score: %d", PlayerScore);
    sprintf(enemyHUD, "Enemy Score: %d", EnemyScore);

    DrawText(Backbuffer, playerHUD, 0, 1, GameFont, 64, 64, 64);
    DrawText(Backbuffer, enemyHUD, 0, 30, GameFont, 64, 64, 64);
}

void FreeGame()
{
    Mix_HaltMusic();    //Stop the music
    FreeFiles();        //Release the files we loaded
    Mix_CloseAudio();   //Close the audio system
    TTF_Quit();         //Close the font system
    SDL_Quit();         //Close SDL
}
