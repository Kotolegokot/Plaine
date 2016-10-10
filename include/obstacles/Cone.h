#ifndef CONE_H
#define CONE_H

#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define CONE_MODEL "media/models/cone.obj"

class Cone : public IBody
{
public:
    Cone(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius, btScalar height) :
        IBody(world, device, position), radius(radius), height(height)
    {
        createBody();
    }

    virtual btScalar getMass()
    {
        btScalar k = M_PI / 3;
        return height*radius*radius*k*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(CONE_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(radius * 2, height, radius * 2));
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
            objMesh = new ObjMesh(CONE_MODEL);

        shape = new btConvexHullShape();
        objMesh->setPoints((btConvexHullShape *) shape);
        shape->setLocalScaling(btVector3(radius * 2, height, radius * 2));
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;
    btScalar height;

    static ObjMesh *objMesh;
};

#endif // TETRAHEDRON_H
