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

#include "PlaneFactory.hpp"

void PlaneFactory::finishing_touch(btRigidBody &body) const
{
    body.setUserIndex(1);
    body.setActivationState(DISABLE_DEACTIVATION);
}

btScalar PlaneFactory::mass() const
{
    return 1.0f;
}

std::unique_ptr<btCollisionShape> PlaneFactory::shape() const
{
    Mesh mesh(PLANE_MODEL, 15);

    return std::make_unique<btConvexTriangleMeshShape>(mesh.triangle_mesh().release());
}
