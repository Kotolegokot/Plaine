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

    virtual ~Icosphere2()
    {
        // so that commonShape is not deleted thousand times within IBody::~IBody
        shape = nullptr;
    }

    virtual btScalar getMass()
    {
        return radius*radius*radius*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(ICOSPHERE2_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(200, 200, 200));
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
        if (!commonShape) {
            commonShape = new btConvexHullShape();
            ObjMesh(ICOSPHERE2_MODEL).setPoints(commonShape);
            commonShape->setLocalScaling(btVector3(200, 200 ,200));
        }

        shape = commonShape;
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;

    static btConvexHullShape *commonShape;
};

#endif // ICOSPHERE2_H
