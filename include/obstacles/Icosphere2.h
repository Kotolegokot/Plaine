#ifndef ICOSPHERE2_H
#define ICOSPHERE2_H

#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define ICOSPHERE2_MODEL "media/models/icosphere2.obj"

class Icosphere2 : public IBody
{
public:
    Icosphere2(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius) :
        IBody(world, device, position), radius(radius)
    {
        createBody();
    }

    virtual btScalar getMass()
    {
        btScalar k = 4 / 3 * M_PI;
        return radius*radius*radius*k*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(ICOSPHERE2_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(radius, radius, radius) * 2);
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));

        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    }

    virtual void createMotionState()
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual void createShape()
    {
        if (!objMesh)
            objMesh = new ObjMesh(ICOSPHERE2_MODEL);

        shape = new btConvexHullShape();
        objMesh->setPoints((btConvexHullShape *) shape);
        shape->setLocalScaling(btVector3(radius, radius, radius) * 2);
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;

    static ObjMesh *objMesh;
};

#endif // ICOSPHERE2_H
