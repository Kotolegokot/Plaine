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

#include <memory>
#include <thread>
#include <queue>
#include <list>
#include <SFML/Audio.hpp>
#include "Log.hpp"
#include "util/Vector3.hpp"

#define COLLISION_FILE "media/sounds/collision.ogg"
#define EXPLOSION_FILE "media/sounds/explosion.ogg"
#define BACKGROUND_FILE "media/sounds/background.ogg"
#define MENU_FILE "media/sounds/menu.ogg"

class Audio {
    Audio();

    sf::SoundBuffer collisionBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer backgroundBuffer;
    sf::SoundBuffer menuBuffer;

    static std::queue<sf::Sound, std::list<sf::Sound>> queue;
    static void clearQueue();

    static Audio instance;
public:
    static Audio &getInstance()
    {
        return instance;
    }

    sf::Sound collision() const;
    sf::Sound explosion() const;
    sf::Sound background() const;
    sf::Sound menu() const;

    static void play(sf::Sound sound, float volume = -1);
    static void playAt(sf::Sound sound, const Vector3<float> &position, float volume = -1);
};

#endif // AUDIO_H
