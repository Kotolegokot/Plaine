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
    Cone(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius, btScalar length) :
        IBody(world, device, position), radius(radius)
    {
        createBody();
    }

    virtual ~Cone()
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
        mesh = device->getSceneManager()->getMesh(CONE_MODEL);
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
            ObjMesh(CONE_MODEL).setPoints(commonShape);
            commonShape->setLocalScaling(btVector3(200, 200 ,200));
        }

        shape = commonShape;
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;
    btScalar length;

    static btConvexHullShape *commonShape;
};

#endif // TETRAHEDRON_H
