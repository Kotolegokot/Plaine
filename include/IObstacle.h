#ifndef IOBSTACLE_H
#define IOBSTACLE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class IObstacle
{
public:
    IObstacle(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
        world(world), device(device), position(position) {}
    virtual ~IObstacle() {}

    btVector3 getPosition() const
    {
        return position;
    }

    void setPosition(const btVector3 &position)
    {
        this->position = position;
    }

protected:
    btDynamicsWorld *world = nullptr;
    IrrlichtDevice *device = nullptr;

    btVector3 position;
};

#endif // IOBSTACLE_H
