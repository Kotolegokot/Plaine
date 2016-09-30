#include "Plane.h"

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world, (shape = new btSphereShape(SPHERE_RADIUS))), device(device), position(position)
{
    createBody();
}

void Plane::createNode()
{
    node = device->getSceneManager()->addSphereSceneNode(SPHERE_RADIUS, 16, 0, -1, core::vector3df(0, 0, 0));
    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));
}

void Plane::createMotionState()
{
    motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
}

btScalar Plane::getMass()
{
        return 1;
}
