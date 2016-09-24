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
    #ifdef _WIN32
        std::string str = "LANGUAGE=";
        str += language;
        _wputenv(utf8_to_wide(str).c_str());
    #else
        setenv("LANGUAGE", language.c_str(), false);
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
