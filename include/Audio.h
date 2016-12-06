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

#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Log.h"

#define COLLISION_FILE "media/sounds/collision.ogg"
#define BACKGROUND_FILE "media/sounds/background.ogg"

class Audio {
    Audio() {
        if (!collision.openFromFile(COLLISION_FILE))
            Log::warning("couldn't open file '", COLLISION_FILE, "'");
        if (!background.openFromFile(BACKGROUND_FILE))
            Log::warning("couldn't open file '", BACKGROUND_FILE, "'");
    }
public:
    static Audio &getInstance()
    {
        static Audio instance;
        return instance;
    }

    sf::Music collision;
    sf::Music background;
};

#endif // AUDIO_H
