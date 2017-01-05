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

#include "Audio.hpp"

Audio Audio::instance;
std::queue<sf::Sound, std::list<sf::Sound>> Audio::queue;

Audio::Audio() {
    if (!collisionBuffer.loadFromFile(COLLISION_FILE))
        Log::getInstance().warning("couldn't open file '", COLLISION_FILE, "'");

    if (!explosionBuffer.loadFromFile(EXPLOSION_FILE))
        Log::getInstance().warning("couldn't open file '", EXPLOSION_FILE, "'");

    if (!backgroundBuffer.loadFromFile(BACKGROUND_FILE))
        Log::getInstance().warning("couldn't open file '", BACKGROUND_FILE, "'");

    if (!menuBuffer.loadFromFile(MENU_FILE))
        Log::getInstance().warning("couldn't open file '", MENU_FILE, "'");
}

sf::Sound Audio::collision() const
{
    return sf::Sound(collisionBuffer);
}

sf::Sound Audio::explosion() const
{
    return sf::Sound(explosionBuffer);
}

sf::Sound Audio::background() const
{
    return sf::Sound(backgroundBuffer);
}

sf::Sound Audio::menu() const
{
    return sf::Sound(menuBuffer);
}

void Audio::play(sf::Sound sound, float volume)
{
    if (volume >= 0)
        sound.setVolume(volume);

    queue.push(sound);
    queue.back().play();

    clearQueue();
}

void Audio::playAt(sf::Sound sound, const Vector3<float> &position, float volume)
{
    sound.setPosition(position);
    if (volume >= 0)
        sound.setVolume(volume);

    queue.push(sound);
    queue.back().play();

    clearQueue();
}

void Audio::clearQueue()
{
    while (queue.front().getStatus() == sf::SoundSource::Stopped)
        queue.pop();
}
