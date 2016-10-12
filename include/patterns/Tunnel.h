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

#ifndef TUNNEL_H
#define TUNNEL_H

#include "obstacles/Box.h"
#include "IObstacle.h"
#include "IObstaclePattern.h"

class Tunnel : public IObstaclePattern
{
public:
    Tunnel(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius,
        btScalar length) :
        IObstaclePattern(world, device, position), radius(radius), length(length)
    {
        boxes[0] = new Box(world, device, position + btVector3(radius, 0, 0), btVector3(radius / 10, radius, length / 2));
        boxes[1] = new Box(world, device, position + btVector3(-radius, 0, 0), btVector3(radius /10, radius, length / 2));
        boxes[2] = new Box(world, device, position + btVector3(0, radius, 0), btVector3(radius, radius / 10, length / 2));
        boxes[3] = new Box(world, device, position + btVector3(0, -radius, 0), btVector3(radius, radius / 10, length / 2));
    }

    void addObstaclesToDeque(std::deque<IObstacle *> &deque) override
    {
        for (int i = 0; i < 4; i++)
            deque.push_back(boxes[i]);
    }

    size_t getObstacleCount() const override
    {
        return 4;
    }

protected:
    btScalar radius = 0;
    btScalar length = 0;

    Box *boxes[4] = { 0, 0, 0, 0 };
    btPoint2PointConstraint *constraint = nullptr;
};

#endif // TUNNEL_H
