/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "IBodyFactory.hpp"
#include "util/Vector3.hpp"
#include "util/constants.hpp"

class BoxProducer : public IBodyFactory {
public:
    BoxProducer(const btVector3 &halfExtents) :
        m_half_extents(halfExtents) {}

    btScalar mass() const override
    {
        return 8*m_half_extents.x()*m_half_extents.y()*m_half_extents.z()*MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<btCollisionShape> shape() const override
    {
        return std::make_unique<btBoxShape>(m_half_extents);
    }

private:
    btVector3 m_half_extents;
};
