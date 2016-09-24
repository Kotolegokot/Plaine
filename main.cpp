#include <iostream>
#include <clocale>
#include <irrlicht.h>
#include <stdlib.h>

#include "Game.h"
#include "Config.h"
#include "util.h"

using namespace irr;

void setLanguage(std::string language)
{
    std::string str = "LANGUAGE=";
    str += language;
    #ifdef _WIN32
        _wputenv(utf8_to_wide(str).c_str());
    #else
        putenv(str.c_str());
    #endif // _WIN32
}

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
