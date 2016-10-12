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

#ifndef IOBSTACLE_H
#define IOBSTACLE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace irr;

class IObstacle
{
public:
    IObstacle(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
        world(world), device(device), position(position) {}
    virtual ~IObstacle() {}

    virtual btVector3 getPosition() const = 0;
    virtual void setPosition(const btVector3 &position) = 0;

protected:
    btDynamicsWorld *world = nullptr;
    IrrlichtDevice *device = nullptr;
    btVector3 position;
};

#endif // IOBSTACLE_H
