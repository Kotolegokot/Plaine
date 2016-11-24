#include "Body.h"

Body::~Body()
{
    if (m_rigidBody)
        m_physicsWorld.removeCollisionObject(m_rigidBody.get());
}

scene::ISceneNode &Body::node()
{
    return dynamic_cast<MotionState *>(m_rigidBody->getMotionState())->getNode();
}

const scene::ISceneNode &Body::node() const
{
    return dynamic_cast<MotionState *>(m_rigidBody->getMotionState())->getNode();
}

btVector3 Body::getPosition() const
{
    return m_rigidBody->getCenterOfMassTransform().getOrigin();
}

void Body::setPosition(const btVector3 &position)
{
    notNanAssert(position);
    btTransform transform = m_rigidBody->getCenterOfMassTransform();
    transform.setOrigin(position);

    m_rigidBody->setCenterOfMassTransform(transform);
}

btVector3 Body::getLinearVelocity() const
{
    return m_rigidBody->getLinearVelocity();
}

void Body::setLinearVelocity(const btVector3 &linearVelocity)
{
    notNanAssert(linearVelocity);
    m_rigidBody->setLinearVelocity(linearVelocity);
}

btScalar Body::getScalarLinearVelocity() const
{
    return m_rigidBody->getLinearVelocity().length();
}

void Body::setScalarLinearVelocity(btScalar length)
{
    notNanAssert(length);
    btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
    linearVelocity.safeNormalize();

    m_rigidBody->setLinearVelocity(linearVelocity * length);
}

btVector3 Body::getAngularVelocity() const
{
    return m_rigidBody->getAngularVelocity();
}

void Body::setAngularVelocity(const btVector3 &angularVelocity)
{
    notNanAssert(angularVelocity);
    m_rigidBody->setAngularVelocity(angularVelocity);
}

btScalar Body::getScalarAngularVelocity() const
{
    return m_rigidBody->getAngularVelocity().length();
}

void Body::setScalarAngularVelocity(btScalar length)
{
    notNanAssert(length);
    btVector3 angularVelocity = m_rigidBody->getAngularVelocity();
    angularVelocity.safeNormalize();

    m_rigidBody->setAngularVelocity(angularVelocity * length);
}

btQuaternion Body::getRotation() const
{
    return m_rigidBody->getCenterOfMassTransform().getRotation();
}

void Body::setRotation(const btQuaternion &rotation)
{
    notNanAssert(rotation);
    const btTransform &transform = m_rigidBody->getCenterOfMassTransform();

    m_rigidBody->setCenterOfMassTransform(btTransform(rotation, transform.getOrigin()));
}

btVector3 Body::getEulerRotationRad() const
{
    return irrlicht2bullet(quatToEulerRad(this->getRotation()));
}

btVector3 Body::getEulerRotationDeg() const
{
    return irrlicht2bullet(quatToEulerDeg(this->getRotation()));
}

void Body::setEulerRotationRad(const btVector3 &rotation)
{
    notNanAssert(rotation);
    const btTransform &transform = m_rigidBody->getCenterOfMassTransform();
    btQuaternion qRotation(rotation.x(), rotation.y(), rotation.z());

    m_rigidBody->setCenterOfMassTransform(btTransform(qRotation, transform.getOrigin()));
}

void Body::setEulerRotationDeg(const btVector3 &rotation)
{
    setEulerRotationRad(rotation * core::DEGTORAD);
}

void Body::getAxisAngleRotation(btVector3 &axis, btScalar &angle) const
{
    const btTransform &transform = m_rigidBody->getCenterOfMassTransform();

    axis = transform.getRotation().getAxis();
    angle = transform.getRotation().getAngle();
}

void Body::setAxisAngleRotation(const btVector3 &axis, btScalar angle)
{
    notNanAssert(axis, angle);
    const btTransform &transform = m_rigidBody->getCenterOfMassTransform();
    btQuaternion qRotation(axis, angle);

    m_rigidBody->setCenterOfMassTransform(btTransform(qRotation, transform.getOrigin()));
}
