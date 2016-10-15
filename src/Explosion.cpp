#include "Explosion.h"

Explosion::Explosion(btDynamicsWorld &world, const btVector3 &position, btScalar radius) :
    world(world)
{
    explosionObject = std::make_unique<btGhostObject>();
    explosionObject->setCollisionShape(new btSphereShape(radius));
    explosionObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
    explosionObject->setCollisionFlags(explosionObject->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

    world.addCollisionObject(explosionObject.get());
    world.getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);
}

Explosion::~Explosion()
{
    world.removeCollisionObject(explosionObject.get());
}

void Explosion::setPosition(const btVector3 &position)
{
    explosionObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
}

btVector3 Explosion::getPosition() const
{
    return explosionObject->getWorldTransform().getOrigin();
}

void Explosion::explode()
{
    #if DEBUG_OUTPUT
        std::cout << "ACHTUNG! VZORVALSYA!" << std::endl;
    #endif // DEBUG_OUTPUT

    for (int i = 0; i < explosionObject->getNumOverlappingObjects(); i++)
    {
        btRigidBody &body = *dynamic_cast<btRigidBody *>(explosionObject->getOverlappingObject(i));
        if (body.getUserIndex() == 1) continue; // if plane, skip it

        //btVector3 force = body.getCenterOfMassPosition() - this->getPosition();
        //force *= 50000.0f / force.length();
        //body.applyForce(force, btVector3(0, 0, 0));

        btVector3 impulse = body.getCenterOfMassPosition() - this->getPosition();
        btScalar impulseLength = impulse.length();
        impulse.safeNormalize();
        impulse *= 1000000 / impulseLength;
        body.applyCentralImpulse(impulse);
    }
}
