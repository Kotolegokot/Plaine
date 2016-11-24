#ifndef IBODY_PRODUCER
#define IBODY_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include "MotionState.h"
#include "Body.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

class IBodyProducer {
public:
    IBodyProducer() = default;

    std::unique_ptr<Body> produce(btDynamicsWorld &physicsWorld,
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
