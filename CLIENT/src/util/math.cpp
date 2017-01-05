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

#include "util/math.hpp"

core::vector3df quatToEulerRad(const btQuaternion &quat)
{
    core::vector3df result;
    core::quaternion(quat.x(), quat.y(), quat.z(), quat.w()).toEuler(result);

    return result;
}

core::vector3df quatToEulerDeg(const btQuaternion &quat)
{
    core::vector3df result;
    core::quaternion(quat.x(), quat.y(), quat.z(), quat.w()).toEuler(result);
    result *= core::RADTODEG;

    return result;
}
