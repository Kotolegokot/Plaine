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

#ifndef OTHER_H
#define OTHER_H

#include <cstdlib>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "util/Randomizer.h"
#include "options.h"

using namespace irr;

constexpr f32 COLOR_CHANGE_INTERVAL = 3000.0f;
video::SColor iridescentColor(const u32 &currentTime);

template <typename Num>
btVector3 irrlicht2bullet(const core::vector3d<Num> &irrVector)
{
    return btVector3(irrVector.X, irrVector.Y, irrVector.Z);
}

core::vector3df bullet2irrlicht(const btVector3 &bulletVector);


#endif // OTHER_H
