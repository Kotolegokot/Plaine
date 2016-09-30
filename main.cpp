#include <iostream>
#include <clocale>
#include <cstdlib>
#include <random>
#include <irrlicht.h>

#include "Game.h"
#include "Config.h"
#include "util.h"

using namespace irr;

int main()
{
    //create structure for config data
    ConfigData data;
    //create object for config
    Config conf;
    //load config into structure for configuration data (conf file lies in root directory of project)
    data = conf.loadConfig("game.conf");
    //setting language (platform dependent realization!)
    setLanguage(data.language, false);

    setlocale(LC_ALL, "");
    bindtextdomain("planerunner", "media/locale/");
    bind_textdomain_codeset("planerunner", "utf-8");
    textdomain("planerunner");
    //create an object with configuration data that loaded from config
    Game game(data);
    game.menu();

    return 0;
}
