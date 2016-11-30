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

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <array>
#include <cstdlib>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "Randomizer.h"
#include "options.h"

using namespace irr;

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;

constexpr btScalar MASS_COEFFICIENT = 0.000002;

template<typename T>
constexpr T PI = T(3.1415926535897932385);

// shit making it possible to compile with TDM-GCC under Windows
template<>
constexpr float PI<float> = 3.1415926535897932385;

video::SColor iridescentColor(const u32 &currentTime);
core::vector3df quatToEulerRad(const btQuaternion &quat);
core::vector3df quatToEulerDeg(const btQuaternion &quat);

template <typename Num>
btVector3 irrlicht2bullet(const core::vector3d<Num> &irrVector)
{
    return btVector3(irrVector.X, irrVector.Y, irrVector.Z);
}

core::vector3df bullet2irrlicht(const btVector3 &bulletVector);

template <typename Number>
int sign(Number num)
{
    if (num < 0)
        return -1;
    else if (num > 0)
        return 1;
    else
        return 0;
}

constexpr f32 COLOR_CHANGE_INTERVAL = 3000.0f;

#endif // UTIL_H_INCLUDED
