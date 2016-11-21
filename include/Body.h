#ifndef BODY_H
#define BODY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <MotionState.h>

using namespace irr;

class Body {
    Body(btDynamicsWorld &physicsWorld, std::unique_ptr<btRigidBody> rigidBody) :
        m_physicsWorld(physicsWorld), m_rigidBody(rigidBody) {}

    ~Body()
    {
        if (m_rigidBody)
            m_physicsWorld.removeRigidBody(m_rigidBody.get());
    }

    btRigidBody &rigidBody()
    {
        return *m_rigidBody;
    }

    const btRigidBody &rigidBody() const
    {
        return *m_rigidBody;
    }

    scene::ISceneNode &node()
    {
        return dynamic_cast<MotionState *>(m_rigidBody->getMotionState())->getNode();
    }

    const scene::ISceneNode &node() const
    {
        return dynamic_cast<MotionState *>(m_rigidBody->getMotionState())->getNode();
    }

    btVector3 getPosition() const override
    {
        return rigidBody->getCenterOfMassTransform().getOrigin();
    }

    void setPosition(const btVector3 &position) override
    {
        btTransform transform = rigidBody->getCenterOfMassTransform();
        transform.setOrigin(position);

        rigidBody->setCenterOfMassTransform(transform);
    }

private:
    std::unique_ptr<btRigidBody> m_rigidBody;
    btDynamicsWorld &m_physicsWorld;
};

#endif // BODY_H
