#ifndef IBODY_PRODUCER
#define IBODY_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include "Body.h"
#include "util/Vector3.h"

using namespace irr;

class IBodyProducer {
    IBodyProducer() = default;

    std::unique_ptr<Body> produce(btDynamicsWorld &physicsWorld,
                                  IrrlichtDevice &irrlichtDeivce,
                                  Vector3<long> &cell)
    {
        auto motionState = createMotionState(createNode(irrlichtDeivce));
        auto shape = createShape();
        btScalar mass = getMass();

        btVector3 inertia(0, 0, 0);
        if (mass)
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState.release(),
                                                             shape.release(), inertia);

        auto rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);
        rigidBody->setUserIndex(0); // default index for bodies
        physicsWorld.addRigidBody(rigidBody.get());

        return Body(physicsWorld, std::move(rigidBody));
    }

    virtual btScalar getMass() = 0;

protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &) = 0;
    virtual std::unique_ptr<btMotionState>
                        createMotionState(std::unique_ptr<scene::ISceneNode>) = 0;
    virtual std::unique_ptr<btCollisionShape> createShape() = 0;
};

#endif // IBODY_PRODUCER
