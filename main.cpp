#include "include/Game.hpp"

int main()
{
    Game* game = new Game(WIDTH, HEIGHT);

    game->init();

    game->main_loop();

    delete game;
    return 0;
}
