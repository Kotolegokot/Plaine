/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#include <clocale>
#include <random>
#include <exception>
#include <irrlicht.h>

#include "Game.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "util/i18n.hpp"

using namespace irr;

int main()
{
    // load config into the structure (conf file is kept in the root directory of the project)
    ConfigData data = Config::loadConfig("game.conf");
    // setting language (platform dependent realization!)
    setLanguage(data.language, false);

    // some magic lines for gettext (no need to understand what they mean)
    setlocale(LC_ALL, "");
    bindtextdomain("planerunner", "media/locale/");
    bind_textdomain_codeset("planerunner", "utf-8");
    textdomain("planerunner");

    // create instance of game class and give it configuration structure
    try {
        Game(data).start();
    } catch (const std::exception &e) {
        Log::getInstance().error(e.what());

        return 1;
    }

    return 0;
}
