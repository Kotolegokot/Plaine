#ifndef TORUS_H
#define TORUS_H

#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define TORUS_MODEL "media/models/torus.obj"

class Torus : public IBody
{
public:
    Torus(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, f32 radius) :
        IBody(world), device(device), position(position), radius(radius)
    {
        createBody();
    }

    virtual btScalar getMass()
    {
        return radius*radius*radius*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(TORUS_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(radius, radius, radius));
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));
    }

    virtual void createMotionState()
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual void createShape()
    {
        ObjMesh objMesh;
        objMesh.loadMesh(TORUS_MODEL);

        shape = new btConvexTriangleMeshShape(objMesh.getTriangleMesh());
        shape->setLocalScaling(btVector3(radius, radius, radius));
    }


private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
    f32 radius;
    scene::IMesh *mesh = nullptr;
};

#endif // TORUS_H
