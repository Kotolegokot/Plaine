#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <cmath>
#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define TETRAHEDRON_MODEL "media/models/tetrahedron.obj"

class Tetrahedron : public IBody
{
public:
    Tetrahedron(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar edge) :
        IBody(world, device, position), edge(edge)
    {
        createBody();
    }

    virtual btScalar getMass()
    {
        constexpr btScalar k = 1 / 6 / std::sqrt(2);
        return edge * edge * edge * k * MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(TETRAHEDRON_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(edge, edge, edge));
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
            objMesh = new ObjMesh(TETRAHEDRON_MODEL);

        shape = new btConvexHullShape();
        objMesh->setPoints((btConvexHullShape *) shape);
        shape->setLocalScaling(btVector3(edge, edge, edge));
    }

private:
    scene::IMesh *mesh = nullptr;
    btScalar edge;

    static ObjMesh *objMesh;
};

#endif // TETRAHEDRON_H
