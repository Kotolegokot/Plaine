#ifndef TORUS_H
#define TORUS_H

#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define TORUS_MODEL "media/models/torus.obj"

class Torus : public IBody
{
public:
    Torus(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius) :
        IBody(world, device, position), radius(radius)
    {
        createBody();
    }

    virtual ~Torus()
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
        mesh = device->getSceneManager()->getMesh(TORUS_MODEL);
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
            commonShape = new btConvexTriangleMeshShape(ObjMesh(TORUS_MODEL).getTriangleMesh());
            commonShape->setLocalScaling(btVector3(200, 200, 200));
        }

        shape = commonShape;
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;

    static btCollisionShape *commonShape;
};

#endif // TORUS_H
