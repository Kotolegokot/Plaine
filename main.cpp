#include <iostream>
#include <irrlicht.h>

using namespace irr;

#include "Game.h"
#include "Config.h"
#include "util.h"

int main()
{
    setlocale(LC_ALL, "");
    bindtextdomain("planerunner", "media/locale/");
    textdomain("planerunner");

    ConfigData data;
    Config conf;
    data = conf.loadConfig("game.conf");

    Game game(data);
    game.menu();

    return 0;
}
