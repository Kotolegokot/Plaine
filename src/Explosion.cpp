#include "Explosion.h"

using namespace irr;

Explosion::Explosion(btDynamicsWorld &world, IrrlichtDevice &device,
                     const btVector3 &position, btScalar radius) :
    world(world), device(device)
{
    explosionObject = std::make_unique<btGhostObject>();
    explosionObject->setCollisionShape(new btSphereShape(radius));
    explosionObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
    explosionObject->setCollisionFlags(explosionObject->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

    world.addCollisionObject(explosionObject.get());
    world.getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);

    particleSystem =
        std::unique_ptr<scene::IParticleSystemSceneNode>(device.getSceneManager()->addParticleSystemSceneNode(false));
    auto particleAffector =
        std::unique_ptr<scene::IParticleAffector>(particleSystem->createFadeOutParticleAffector());
    particleSystem->addAffector(particleAffector.get());
    particleAffector.release()->drop();
}

Explosion::~Explosion()
{
    world.removeCollisionObject(explosionObject.get());
    particleSystem.release()->remove();
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

    startAnimation();

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

void Explosion::startAnimation()
{
    auto emitter =
        std::unique_ptr<scene::IParticleEmitter>(particleSystem->createBoxEmitter(
            core::aabbox3d<f32>(-7, 7, -7, 7, 7, 7), // emitter size
            core::vector3df(0.0f, 0.06f, 0.0f),      // initial direction
            80, 100,                                 // emit rate
            video::SColor(0, 255, 255, 255),         // darkest color
            video::SColor(0, 255, 255, 255),         // brightest color
            800, 2000, 0,                            // min and max age, ange
            core::dimension2df(10.0f, 10.0f),        // min size
            core::dimension2df(20.0f, 20.0f)));       // max size

    particleSystem->setEmitter(emitter.get());
    emitter.release()->drop();

    const auto &position = getPosition();
    particleSystem->setPosition(core::vector3df(position.x(), position.y(), position.z()));
    particleSystem->setScale(core::vector3df(2, 2, 2));
    particleSystem->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    particleSystem->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/lsd.png"));
    particleSystem->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}
