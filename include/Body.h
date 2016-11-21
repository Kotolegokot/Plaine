#ifndef BODY_H
#define BODY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <MotionState.h>

using namespace irr;

class Body {
public:
    Body(btDynamicsWorld &physicsWorld, std::unique_ptr<btRigidBody> rigidBody) :
        m_physicsWorld(physicsWorld), m_rigidBody(std::move(rigidBody)) {}

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

    btVector3 getPosition() const
    {
        return m_rigidBody->getCenterOfMassTransform().getOrigin();
    }

    void setPosition(const btVector3 &position)
    {
        btTransform transform = m_rigidBody->getCenterOfMassTransform();
        transform.setOrigin(position);

        m_rigidBody->setCenterOfMassTransform(transform);
    }

private:
    btDynamicsWorld &m_physicsWorld;
    std::unique_ptr<btRigidBody> m_rigidBody;
};

#endif // BODY_H
