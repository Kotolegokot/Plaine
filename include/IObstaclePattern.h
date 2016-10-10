#ifndef IOBSTACLEPATTERN_H
#define IOBSTACLEPATTERN_H

#include <deque>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <IObstacle.h>

using namespace irr;

class IObstaclePattern
{
public:
    IObstaclePattern(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
        world(world), device(device), position(position) {}

    virtual void addObstaclesToDeque(std::deque<IObstacle *> &deque) = 0;

protected:
    btDynamicsWorld *world = nullptr;
    IrrlichtDevice *device = nullptr;
    btVector3 position;

};

#endif // IOBSTACLEPATTERN_H
