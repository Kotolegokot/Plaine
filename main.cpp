#include <iostream>
#include <clocale>
#include <irrlicht.h>
#include <stdlib.h>

#include "Game.h"
#include "Config.h"
#include "util.h"

using namespace irr;

int main()
{
    ConfigData data;
    Config conf;
    data = conf.loadConfig("game.conf");
    setLanguage(data.language);

    setlocale(LC_ALL, "");
    bindtextdomain("planerunner", "media/locale/");
    bind_textdomain_codeset("planerunner", "utf-8");
    textdomain("planerunner");

    Game game(data);
    game.menu();

    return 0;
}
