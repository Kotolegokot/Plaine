#include "Plane.h"

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world), device(device), position(position)
{
    createBody();
}

void Plane::createNode()
{
    planeMesh = device->getSceneManager()->getMesh("media/models/plane.obj");
    node = device->getSceneManager()->addMeshSceneNode(planeMesh);
    node->setScale(core::vector3df(15, 15, 15));
    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/plane.png"));
}

void Plane::createMotionState()
{
    motionState = new PlaneMotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
}

void Plane::createShape()
{
    shape = new btSphereShape(SPHERE_RADIUS);
}

btScalar Plane::getMass()
{
        return 1;
}
