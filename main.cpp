#include "include/Game.hpp"

int main(int argc, char* argv[])
{
    Game* game = new Game(WIDTH, HEIGHT, true);

    //game->init();

    if(argc > 1)
    {
        if(!strcmp(argv[1], "--debug"))
        {
            game->increase_money(100000);
            game->switch_debug();
        }
    }

    game->main_loop();
    

    delete game;
    return 0;
}
