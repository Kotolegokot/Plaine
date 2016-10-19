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

#ifndef IOBSTACLEPATTERN_H
#define IOBSTACLEPATTERN_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <IObstacle.h>

using namespace irr;

class IObstaclePattern
{
public:
    IObstaclePattern(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position) :
        world(world), device(device), position(position) {}
    virtual ~IObstaclePattern() {}

    virtual void addObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) = 0;
    virtual size_t getObstacleCount() const = 0;

protected:
    btDynamicsWorld &world;
    IrrlichtDevice &device;
    btVector3 position;

};

#endif // IOBSTACLEPATTERN_H
