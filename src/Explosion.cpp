#include "Explosion.h"

using namespace irr;

Explosion::Explosion(btDynamicsWorld &world, IrrlichtDevice &device,
                     const btVector3 &position, btScalar radius) :
    world(world), device(device), radius(radius)
{
    explosionObject = std::make_unique<btGhostObject>();
    explosionObject->setCollisionShape(new btSphereShape(radius));
    explosionObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
    explosionObject->setCollisionFlags(explosionObject->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

    world.addCollisionObject(explosionObject.get());
    world.getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);

    /*particleSystem =
        std::unique_ptr<scene::IParticleSystemSceneNode>(device.getSceneManager()->addParticleSystemSceneNode(false));

    auto particleAffector =
        std::unique_ptr<scene::IParticleAffector>(particleSystem->createFadeOutParticleAffector());
    particleSystem->addAffector(particleAffector.get());
    particleAffector.release()->drop();

    particleAffector =
        std::unique_ptr<scene::IParticleAffector>(particleSystem->createScaleParticleAffector(core::dimension2df(5.f, 5.f)));
    particleSystem->addAffector(particleAffector.get());
    particleAffector.release()->drop();*/
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
        std::cout << "PLANE EXPLODED!!" << std::endl;
    #endif // DEBUG_OUTPUT

    //startAnimation();

    for (int i = 0; i < explosionObject->getNumOverlappingObjects(); i++)
    {
        btRigidBody &body = *dynamic_cast<btRigidBody *>(explosionObject->getOverlappingObject(i));
        if (body.getUserIndex() == 1) continue; // if plane, skip it

        btVector3 force = body.getCenterOfMassPosition() - this->getPosition();
        btScalar distance = force.length();
        force.safeNormalize();
        force *= (radius - distance * 0.1f) * 100.0f;
        body.activate();
        body.applyForce(force, btVector3(0, 0, 0));
    }
}

void Explosion::startAnimation()
{
    /*auto emitter =
        std::unique_ptr<scene::IParticleEmitter>(particleSystem->createPointEmitter(
             core::vector3df(0.f, 0.01f, 0.f),   // direction
             80, 100,                          // emit rate
             video::SColor(0, 255, 255, 255),  // min color
             video::SColor(0, 255, 255, 255),  // max color
             2000, 4000, 0,                    // min and max age, angle
             core::dimension2df(10.f, 10.f),   // min size
             core::dimension2df(20.f, 20.f))); // max size

    particleSystem->setEmitter(emitter.get());
    emitter.release()->drop();

    const auto &position = getPosition();
    particleSystem->setPosition(core::vector3df(position.x(), position.y(), position.z()));
    particleSystem->setScale(core::vector3df(20, 20, 20));
    particleSystem->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    particleSystem->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/lsd.png"));
    particleSystem->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);*/
}
