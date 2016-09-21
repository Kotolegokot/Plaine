#include <iostream>
#include <irrlicht.h>

using namespace irr;

#include "Game.h"
#include "Config.h"

int main()
{
    ConfigData data;
    Config conf;
    data = conf.loadConfig("game.conf");

    Game game(data);
    game.menu();

    return 0;
}
