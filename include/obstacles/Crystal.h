#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "obstacles/Cone.h"
#include "IObstacle.h"

class Crystal : public IObstacle
{
public:
    Crystal(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius,
        btScalar length) :
        IObstacle(world, device, position), radius(radius), length(length)
    {
        cone1 = new Cone(world, device, position, radius, length / 2.0f);
        cone2 = new Cone(world, device, position, radius, length / 2.0f);

        // turn cone2 upside down
        btTransform transform;
        cone2->getRigidBody()->getMotionState()->getWorldTransform(transform);
        btQuaternion rotation(0, 0, core::PI);
        transform.setRotation(rotation);
        cone2->getRigidBody()->setCenterOfMassTransform(transform);
    }

    btVector3 getPosition() const override
    {
        return cone1->getPosition();
    }

    void setPosition(const btVector3 &position) override
    {
        cone1->setPosition(position);
        cone2->setPosition(position);
    }

    virtual ~Crystal()
    {
        delete cone1;
        delete cone2;
    }

protected:
    btScalar radius = 0;
    btScalar length = 0;

    Cone *cone1 = nullptr;
    Cone *cone2 = nullptr;
};

#endif // CRYSTAL_H
