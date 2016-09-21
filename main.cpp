#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;

#include "Game.h"
#include "Config.h"

int main()
{
    ConfigData data;
    Config conf;
    conf.saveConfig("config.conf", data);
    data = conf.loadConfig("config.conf");

    Game game(data);
    game.menu();

    return 0;
}
