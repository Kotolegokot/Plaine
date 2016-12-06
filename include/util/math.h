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

#ifndef MATH_H
#define MATH_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

using namespace irr;

template<typename T>
constexpr T PI = T(3.1415926535897932385);

// shit making it possible to compile with TDM-GCC under Windows
template<>
constexpr float PI<float> = 3.1415926535897932385;

core::vector3df quatToEulerRad(const btQuaternion &quat);
core::vector3df quatToEulerDeg(const btQuaternion &quat);

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

#endif // MATH_H
