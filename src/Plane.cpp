#include "Plane.h"

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world), device(device), position(position)
{
    createBody();
}

void Plane::createNode()
{
    planeMesh = device->getSceneManager()->getMesh(PLANE_MODEL);
    node = device->getSceneManager()->addMeshSceneNode(planeMesh);
    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/plane.png"));
}

void Plane::createMotionState()
{
    motionState = new PlaneMotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
}

void Plane::createShape()
{
    ObjMesh objMesh;
    objMesh.loadMesh(PLANE_MODEL);

    shape = new btConvexTriangleMeshShape(objMesh.getTriangleMesh());
}

btScalar Plane::getMass()
{
        return 1;
}
