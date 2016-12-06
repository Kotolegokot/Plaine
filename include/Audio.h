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
#include <thread>
#include "Log.h"
#include "util/Vector3.h"

#define COLLISION_FILE "media/sounds/collision.ogg"
#define BACKGROUND_FILE "media/sounds/background.ogg"

class Audio {
    Audio();

    sf::SoundBuffer collisionBuffer;
    sf::SoundBuffer backgroundBuffer;

    static Audio instance;
public:
    static Audio &getInstance()
    {
        return instance;
    }

    sf::Sound getCollision() const;
    void playCollision(const Vector3<float> &position = { 0, 0, 0}) const;
    sf::Sound getBackground() const;
    void playBlackground() const;
};

#endif // AUDIO_H
