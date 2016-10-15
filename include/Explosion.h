#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class Explosion
{
public:
    Explosion(btDynamicsWorld &world, const btVector3 &position = btVector3(0, 0, 0), btScalar radius = 1000);
    ~Explosion();

    void setPosition(const btVector3 &position);
    btVector3 getPosition() const;

    void explode();

private:
    btDynamicsWorld &world;
    std::unique_ptr<btGhostObject> explosionObject;
};

#endif // EXPLOSION_H
