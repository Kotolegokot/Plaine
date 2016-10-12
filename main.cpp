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
    // create structure for config
    ConfigData data;
    // create config loader
    Config conf;
    // load config into the structure (conf file lies in the root directory of the project)
    data = conf.loadConfig("game.conf");
    // setting language (platform dependent realization!)
    setLanguage(data.language, false);

    // some magic lines for gettext (no need to understand what they mean)
    setlocale(LC_ALL, "");
    bindtextdomain("planerunner", "media/locale/");
    bind_textdomain_codeset("planerunner", "utf-8");
    textdomain("planerunner");

    // create instance of game class and give it configuration structure
    Game game(data);
    // run menu
    game.menu();

    return 0;
}
