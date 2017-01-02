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

#define CONE_MODEL "media/models/cone.obj"

class ConeFactory : public IBodyFactory {
public:
    ConeFactory(btScalar radius, btScalar height) :
        m_radius(radius), m_height(height) {}

    btScalar mass() const override
    {
        static constexpr btScalar K = PI<btScalar> * 1.0f / 3.0f;

        return m_height * m_radius * m_radius * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<btCollisionShape> shape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(mesh.points(), mesh.points_count(),
                                                         btVector3(m_radius * 2, m_height, m_radius * 2));
    }


private:
    const btScalar m_radius;
    const btScalar m_height;

    static Mesh &mesh;
};
