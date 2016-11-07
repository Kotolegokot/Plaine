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
    IObstaclePattern() = default;
    IObstaclePattern(IObstaclePattern &&) = delete;
    IObstaclePattern &operator =(IObstaclePattern &&) = delete;
    IObstaclePattern(const IObstaclePattern &) = delete;
    IObstaclePattern &operator =(const IObstaclePattern &) = delete;
    virtual ~IObstaclePattern() {}

    virtual void getSize(std::size_t &x, std::size_t &y, std::size_t &z) const;

    virtual void moveObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) = 0;
    virtual std::size_t getObstacleCount() const = 0;
};

#endif // IOBSTACLEPATTERN_H
