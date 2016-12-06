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

#include "Audio.h"

Audio Audio::instance;

Audio::Audio() {
    if (!collisionBuffer.loadFromFile(COLLISION_FILE))
        Log::getInstance().warning("couldn't open file '", COLLISION_FILE, "'");
    if (!backgroundBuffer.loadFromFile(BACKGROUND_FILE))
        Log::getInstance().warning("couldn't open file '", BACKGROUND_FILE, "'");
}

sf::Sound Audio::getCollision() const
{
    sf::Sound collision;
    collision.setBuffer(collisionBuffer);

    return collision;
}

void Audio::playCollision(const Vector3<float> &position) const
{
    std::thread([this, &position]{
        auto collision = getCollision();
        collision.setPosition(position);
        collision.play();

        while (collision.getStatus() == sf::SoundSource::Playing) {}
    }).detach();
}

sf::Sound Audio::getBackground() const
{
    sf::Sound background;
    background.setBuffer(backgroundBuffer);

    return background;
}

void Audio::playBlackground() const
{
    std::thread([this]{
        auto bg = getBackground();
        bg.play();

        while (bg.getStatus() == sf::SoundSource::Playing) {}
    }).detach();
}
