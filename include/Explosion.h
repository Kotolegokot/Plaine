#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace std;

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
    std::unique_ptr<btGhostObject> explosionObject;
    std::unique_ptr<scene::IParticleSystemSceneNode> particleSystem;
};

#endif // EXPLOSION_H
