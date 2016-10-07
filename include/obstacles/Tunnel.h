#ifndef TUNNEL_H
#define TUNNEL_H

#include "obstacles/Box.h"
#include "IObstacle.h"

class Tunnel : public IObstacle
{
public:
    Tunnel(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius,
        btScalar length) :
        IObstacle(world, device, position), radius(radius), length(length)
    {
        box1 = new Box(world, device, position + btVector3(radius, 0, 0), btVector3(radius / 10, radius, length / 2));
        box2 = new Box(world, device, position + btVector3(-radius, 0, 0), btVector3(radius /10, radius, length / 2));
        box3 = new Box(world, device, position + btVector3(0, radius, 0), btVector3(radius, radius / 10, length / 2));
        box4 = new Box(world, device, position + btVector3(0, -radius, 0), btVector3(radius, radius / 10, length / 2));
    }

    virtual ~Tunnel()
    {
        delete box1;
        delete box2;
        delete box3;
        delete box4;
    }

protected:
    btScalar radius = 0;
    btScalar length = 0;

    Box *box1 = nullptr;
    Box *box2 = nullptr;
    Box *box3 = nullptr;
    Box *box4 = nullptr;
};

#endif // TUNNEL_H
