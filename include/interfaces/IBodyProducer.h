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

#ifndef IBODY_PRODUCER
#define IBODY_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include "MotionState.h"
#include "Body.h"
#include "util/Vector3.h"
#include "util/other.h"

using namespace irr;

class IBodyProducer {
public:
    IBodyProducer() = default;

    virtual std::unique_ptr<Body> produce(btDynamicsWorld &physicsWorld,
                                          IrrlichtDevice &irrlichtDeivce,
                                          const btVector3 &position = { 0, 0, 0 }) const
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

        return std::make_unique<Body>(physicsWorld, std::move(rigidBody));
    }

    virtual btScalar getMass() const = 0;

    btTransform relativeTransform = btTransform::getIdentity();
protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &IrrlichtDevice,
                                              const btTransform &absoluteTransform) const = 0;

    virtual std::unique_ptr<btCollisionShape> createShape() const = 0;

    virtual void finishingTouch(btRigidBody &/* body */) const {}
};

#endif // IBODY_PRODUCER
