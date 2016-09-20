#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;

#include "Game.h"
#include "Config.h"

int main()
{
    Config config;
    ConfigData data;
    data.language = L"Къырымтатар тили";
    data.resolution.Width = 1000000;
    config.saveConfig("game.conf", data);

    Game game;
    game.menu();

    return 0;
}
