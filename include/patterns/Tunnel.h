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
    Tunnel(btDynamicsWorld &world, IrrlichtDevice &device, btVector3 position,
           std::size_t cellSize)
    {
        const btScalar radius = cellSize * 0.4f;
        const btScalar length = cellSize * 1.8f;
        position += { cellSize * 0.5f, cellSize * 0.5f, cellSize };

        boxes[0] = std::make_unique<Box>(world, device, position + btVector3(radius, 0, 0), btVector3(radius / 10, radius, length / 2));
        boxes[1] = std::make_unique<Box>(world, device, position + btVector3(-radius, 0, 0), btVector3(radius / 10, radius, length / 2));
        boxes[2] = std::make_unique<Box>(world, device, position + btVector3(0, radius, 0), btVector3(radius, radius / 10, length / 2));
        boxes[3] = std::make_unique<Box>(world, device, position + btVector3(0, -radius, 0), btVector3(radius, radius / 10, length / 2));
    }

    void getSize(std::size_t &x, std::size_t &y, std::size_t &z) const override
    {
        x = 1;
        y = 1;
        z = 2;
    }

    void moveObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) override
    {
        for (auto &boxUPtr : boxes)
            list.push_back(std::move(boxUPtr));
    }

    std::size_t getObstacleCount() const override
    {
        return 4;
    }

protected:
    std::array<std::unique_ptr<Box>, 4> boxes;
};

#endif // TUNNEL_H
