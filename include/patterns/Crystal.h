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

#include <memory>
#include "obstacles/Cone.h"
#include "IObstaclePattern.h"

template <int Thickness, int Length>
class Crystal : public IObstaclePattern
{
public:
    Crystal(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { Thickness, Length, Thickness };
    }

    std::size_t produce(btDynamicsWorld &world,
                                IrrlichtDevice &device,
                                btScalar cellSize,
                                btVector3 position,
                                std::list<std::unique_ptr<IObstacle>> &list) const override
    {
        const btScalar radius = (Thickness - 0.6f) * cellSize * 0.5f;
        const btScalar length = (Length - 0.2f) * cellSize;
        position += { Thickness * cellSize * 0.5f,
                      Length * cellSize * 0.5f,
                      Thickness * cellSize * 0.5f };

        auto cone1 = std::make_unique<Cone>(world, device, position, radius, length * 0.5f);
        auto cone2 = std::make_unique<Cone>(world, device, position, radius, length * 0.5f);

        // turn cone2 upside down
        btTransform transform = cone2->getRigidBody().getCenterOfMassTransform();
        btQuaternion rotation(0, 0, PI<btScalar>);
        transform.setRotation(rotation);
        cone2->getRigidBody().setCenterOfMassTransform(transform);

        list.push_back(std::move(cone1));
        list.push_back(std::move(cone2));

        return 2;
    }
};

#endif // CRYSTAL_H
