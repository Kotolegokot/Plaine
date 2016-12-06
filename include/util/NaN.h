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

#ifndef NAN_H
#define NAN_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

using namespace irr;

#if NAN_ASSERT
template <typename Real>
void notNanAssert(Real real)
{
    assert(!std::isnan(real));
}
#else
template <typename Real>
void notNanAssert(Real) {}
#endif

template <typename ...Ts>
void notNanAssert(const Ts &... params)
{
    // this line just call notNanAssert for all params
    (void) (int []) { 0, (notNanAssert(params), 0)... };
}

void notNanAssert(const btQuaternion &quat);
void notNanAssert(const btVector3 &vec);
void notNanAssert(const btTransform &transform);

#endif // NAN_H
