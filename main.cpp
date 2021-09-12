#include "include/Game.hpp"

int main()
{
    Game* game = new Game(WIDTH, HEIGHT);

    game->init();

    //game->addMonkey(glm::vec2(250, 70), DART_MONKEY, true);

    game->main_loop();

    delete game;
    return 0;
}
