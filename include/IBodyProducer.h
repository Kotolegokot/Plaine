#ifndef IBODY_PRODUCER
#define IBODY_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include "Body.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

class IBodyProducer {
public:
    IBodyProducer() = default;

    std::unique_ptr<Body> produce(btDynamicsWorld &physicsWorld,
                                  IrrlichtDevice &irrlichtDeivce,
                                  const btVector3 &position) const
    {
        btVector3 absolutePosition = position + relativePosition;

        auto node = createNode(irrlichtDeivce, bullet2irrlicht(absolutePosition));
        auto motionState = createMotionState(std::move(node), absolutePosition);
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

        return std::make_unique<Body>(physicsWorld, std::move(rigidBody));
    }

    virtual btScalar getMass() const = 0;

    btVector3 relativePosition { 0, 0, 0 };
protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &IrrlichtDevice,
                                                          const core::vector3df &position) const = 0;
    virtual std::unique_ptr<btMotionState>
                        createMotionState(std::unique_ptr<scene::ISceneNode> node,
                                          const btVector3 &position) const = 0;
    virtual std::unique_ptr<btCollisionShape> createShape() const = 0;
};

#endif // IBODY_PRODUCER
