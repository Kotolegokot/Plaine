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

#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include "IBodyFactory.hpp"
#include "Plane.hpp"
#include "Mesh.hpp"

#define PLANE_MODEL "media/models/plane.obj"

struct PlaneFactory : public IBodyFactory
{
    PlaneFactory() = default;
    btScalar mass() const override;

protected:
    std::unique_ptr<btCollisionShape> shape() const override;
    void finishing_touch(btRigidBody &body) const override;
};
