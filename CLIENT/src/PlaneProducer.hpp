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

#ifndef PLANE_PRODUCER_H
#define PLANE_PRODUCER_H

#include <algorithm>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "interfaces/IBodyProducer.hpp"
#include "Body.hpp"
#include "Plane.hpp"
#include "ObjMesh.hpp"
#include "util/other.hpp"

#define PLANE_MODEL "media/models/plane.obj"

class PlaneProducer : public IBodyProducer
{
public:
    PlaneProducer() = default;

    std::unique_ptr<Plane> producePlane(btDynamicsWorld &physicsWorld,
                                        IrrlichtDevice &irrlichtDevice,
                                        const btVector3 &position = { 0, 0, 0 }) const
    {
        auto body = produce(physicsWorld, irrlichtDevice, position);

        return std::unique_ptr<Plane>(static_cast<Plane *>(body.release()));
    }

    std::unique_ptr<Body> produce(btDynamicsWorld &physicsWorld,
                                  IrrlichtDevice &irrlichtDeivce,
                                  const btVector3 &position = { 0, 0, 0 }) const override;

    btScalar getMass() const override;

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                      const btTransform &absoluteTransform) const override;
    std::unique_ptr<btCollisionShape> createShape() const override;
};

#endif // PLANE_PRODUCER_H
