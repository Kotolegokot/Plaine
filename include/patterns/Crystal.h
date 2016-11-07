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

#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "obstacles/Cone.h"
#include "IObstacle.h"
#include "IObstaclePattern.h"
#include "util.h"

class Crystal : public IObstaclePattern
{
public:
    Crystal(btDynamicsWorld &world, IrrlichtDevice &device, btVector3 position,
            std::size_t cellSize)
    {
        const btScalar radius = cellSize * 0.4f;
        const btScalar length = cellSize * 1.8f;
        position += { cellSize * 0.5f, cellSize, cellSize * 0.5f };

        cone1 = std::make_unique<Cone>(world, device, position, radius, length / 2.0f);
        cone2 = std::make_unique<Cone>(world, device, position + btVector3(0, 50, 0), radius, length / 2.0f);

        // turn cone2 upside down
        btTransform transform = cone2->getRigidBody().getCenterOfMassTransform();
        btQuaternion rotation(0, 0, PI<btScalar>);
        transform.setRotation(rotation);
        cone2->getRigidBody().setCenterOfMassTransform(transform);
    }

    void getSize(std::size_t &x, std::size_t &y, std::size_t &z) const override
    {
        x = 1;
        y = 2;
        z = 1;
    }

    void moveObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) override
    {
        list.push_back(std::move(cone1));
        list.push_back(std::move(cone2));
    }

    std::size_t getObstacleCount() const override
    {
        return 2;
    }

protected:
    std::unique_ptr<Cone> cone1, cone2;
};

#endif // CRYSTAL_H
