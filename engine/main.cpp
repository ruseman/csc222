#include "game.h"

int main(int argc, char *argv[])
{
    Game game;

    if(!game.init())
        game.free();

    game.run();

    return 0;
}