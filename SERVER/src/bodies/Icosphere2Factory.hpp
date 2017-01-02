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
#include <BulletCollision/CollisionShapes/btConvexPointCloudShape.h>
#include "IBodyFactory.hpp"
#include "Mesh.hpp"
#include "util/Vector3.hpp"
#include "util/constants.hpp"

#define ICOSPHERE2_MODEL "media/models/icosphere2.obj"

class Icosphere2Factory : public IBodyFactory {
public:
    Icosphere2Factory(btScalar radius) :
        m_radius(radius) {}

    btScalar mass() const override
    {
        static constexpr btScalar K = 4.0f / 3.0f * PI<btScalar>;

        return m_radius * m_radius * m_radius * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<btCollisionShape> shape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(mesh.points(), mesh.points_count(),
                                                         btVector3(m_radius, m_radius, m_radius) * 2);
    }


private:
    const btScalar m_radius;

    static Mesh &mesh;
};
