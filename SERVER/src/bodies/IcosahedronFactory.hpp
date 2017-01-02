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

#include <cmath>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "IBodyFactory.hpp"
#include "Mesh.hpp"
#include "util/Vector3.hpp"
#include "util/constants.hpp"

#define ICOSAHEDRON_MODEL "media/models/icosahedron.obj"

class IcosahedronFactory : public IBodyFactory {
public:
    IcosahedronFactory(btScalar edge) :
        m_edge(edge) {}

    btScalar mass() const override
    {
        static btScalar K = 5.0f / 12.0f * (3.0f + std::sqrt(5.0f));

        return m_edge * m_edge * m_edge * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<btCollisionShape> shape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(mesh.points(), mesh.points_count(),
                                                         btVector3(m_edge, m_edge, m_edge));
    }


private:
    const btScalar m_edge;

    static Mesh &mesh;
};
