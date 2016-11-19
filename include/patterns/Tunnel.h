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
    Tunnel(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { 1, 1, 2 };
    }

    virtual std::size_t produce(btDynamicsWorld &world,
                                IrrlichtDevice &device,
                                btScalar cellSize,
                                btVector3 position,
                                std::list<std::unique_ptr<IObstacle>> &list) const override
    {
        const btScalar radius = cellSize * 0.4f;
        const btScalar length = cellSize * 1.8f;
        position += { cellSize * 0.5f, cellSize * 0.5f, cellSize };

        auto box1 = std::make_unique<Box>(world, device, position + btVector3(radius, 0, 0),
                                          btVector3(radius / 10, radius, length / 2));
        auto box2 = std::make_unique<Box>(world, device, position + btVector3(-radius, 0, 0),
                                          btVector3(radius / 10, radius, length / 2));
        auto box3 = std::make_unique<Box>(world, device, position + btVector3(0, radius, 0),
                                          btVector3(radius, radius / 10, length / 2));
        auto box4 = std::make_unique<Box>(world, device, position + btVector3(0, -radius, 0),
                                          btVector3(radius, radius / 10, length / 2));

        list.push_back(std::move(box1));
        list.push_back(std::move(box2));
        list.push_back(std::move(box3));
        list.push_back(std::move(box4));

        return 4;
    }
};

#endif // TUNNEL_H
