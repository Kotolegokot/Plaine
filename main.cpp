#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;

#include "Game.h"
#include "Config.h"

int main()
{
    Config config;
    config.loadConfig("game.conf");

    Game game;
    game.menu();

    return 0;
}
