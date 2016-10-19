#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <algorithm>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace irr;

class Explosion
{
public:
    Explosion(btDynamicsWorld &world, IrrlichtDevice &device,
              const btVector3 &position = btVector3(0, 0, 0), btScalar radius = 1000);
    ~Explosion();

    void setPosition(const btVector3 &position);
    btVector3 getPosition() const;

    void explode();

private:
    btDynamicsWorld &world;
    IrrlichtDevice &device;
    btScalar radius = 0.0f;
    std::unique_ptr<btGhostObject> explosionObject;
    std::unique_ptr<scene::IParticleSystemSceneNode> particleSystem;


    void startAnimation();
};

#endif // EXPLOSION_H
