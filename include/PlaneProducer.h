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

#ifndef PLANE_PRODUCER_H
#define PLANE_PRODUCER_H

#include <algorithm>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IBodyProducer.h"
#include "Body.h"
#include "Plane.h"
#include "ObjMesh.h"
#include "util.h"

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
                                  const btVector3 &position = { 0, 0, 0 }) const override
    {
        btTransform absoluteTransform = relativeTransform;
        absoluteTransform.getOrigin() += position;

        auto node = createNode(irrlichtDeivce, absoluteTransform);
        node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
        auto motionState = std::make_unique<MotionState>(btTransform::getIdentity(), node.release());
        auto shape = createShape();
        btScalar mass = getMass();

        btVector3 inertia(0, 0, 0);
        if (mass)
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState.release(),
                                                             shape.release(), inertia);

        auto rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);
        rigidBody->setCenterOfMassTransform(absoluteTransform);
        rigidBody->setUserIndex(0); // default index for bodies
        finishingTouch(*rigidBody);
        physicsWorld.addRigidBody(rigidBody.get());

        return std::make_unique<Plane>(physicsWorld, std::move(rigidBody));
    }

    btScalar getMass() const override;

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                      const btTransform &absoluteTransform) const override;
    std::unique_ptr<btCollisionShape> createShape() const override;

    void finishingTouch(btRigidBody &body) const override;
};

#endif // PLANE_PRODUCER_H
