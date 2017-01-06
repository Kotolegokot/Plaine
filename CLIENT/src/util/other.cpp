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

#include "util/other.hpp"

using namespace irr;

video::SColor iridescentColor(const u32 &currentTime)
{
    static video::SColor color = video::SColor(0, Randomizer::get_float(0, 255), Randomizer::get_float(0, 255), Randomizer::get_float(0, 255));
    static video::SColor oldColor = color;
    static f32 diff = 0;
    static u32 time = 0;
    if (int(time - currentTime) > COLOR_CHANGE_INTERVAL)
        time = currentTime + COLOR_CHANGE_INTERVAL;
    if (currentTime >= time)
        {
            oldColor = color;
            color = video::SColor(0, Randomizer::get_float(100, 255), Randomizer::get_float(100, 255), Randomizer::get_float(100, 255));
            time = currentTime + COLOR_CHANGE_INTERVAL;
        }
    diff = (time - currentTime)/COLOR_CHANGE_INTERVAL;

    return video::SColor(0, color.getRed() + int((oldColor.getRed()) - (color.getRed()))*diff,
                                    color.getGreen() + int(oldColor.getGreen() - color.getGreen())*diff,
                                    color.getBlue() + int(oldColor.getBlue() - color.getBlue())*diff);
}

core::vector3df bullet2irrlicht(const btVector3 &bulletVector)
{
    return core::vector3df(bulletVector.x(), bulletVector.y(), bulletVector.z());
}
