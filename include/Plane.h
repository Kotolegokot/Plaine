#ifndef PLANE_H
#define PLANE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IBody.h"

#define SPHERE_RADIUS 50
#define CAMERA_DISTANCE 200
#define PLANE_MASS 1

class Plane : public IBody
{
public:
    Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
        IBody(world, (shape = new btSphereShape(SPHERE_RADIUS))), device(device), position(position)
    {
        createBody();
    }

protected:
    virtual void createNode() override
    {
        node = device->getSceneManager()->addSphereSceneNode(SPHERE_RADIUS, 16, 0, -1, core::vector3df(0, 0, 0));
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));
    }

    virtual void createMotionState() override
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual btScalar getMass() override
    {
        return 1;
    }

private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
};

#endif // PLANE_H
