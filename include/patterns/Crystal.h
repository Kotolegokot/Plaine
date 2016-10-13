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

class Crystal : public IObstaclePattern
{
public:
    Crystal(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius,
        btScalar length) :
        IObstaclePattern(world, device, position), radius(radius), length(length)
    {
        cone1 = std::make_unique<Cone>(world, device, position, radius, length / 2.0f);
        cone2 = std::make_unique<Cone>(world, device, position, radius, length / 2.0f);

        // turn cone2 upside down
        btTransform transform;
        cone2->getRigidBody().getMotionState()->getWorldTransform(transform);
        btQuaternion rotation(0, 0, core::PI);
        transform.setRotation(rotation);
        cone2->getRigidBody().setCenterOfMassTransform(transform);
    }

    void addObstaclesToDeque(std::deque<std::unique_ptr<IObstacle>> &deque) override
    {
        deque.push_back(std::move(cone1));
        deque.push_back(std::move(cone2));
    }

    size_t getObstacleCount() const override
    {
        return 2;
    }

protected:
    btScalar radius = 0;
    btScalar length = 0;

    std::unique_ptr<Cone> cone1, cone2;
};

#endif // CRYSTAL_H
