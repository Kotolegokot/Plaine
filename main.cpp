/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

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
