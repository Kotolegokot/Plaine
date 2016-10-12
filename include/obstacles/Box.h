#ifndef BOX_H
#define BOX_H

#include "IBody.h"

#define MASS_COEFFICIENT 0.000002

using namespace irr;

class Box : public IBody
{
public:
    Box(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, const btVector3 &halfExtents) :
        IBody(world, device, position), halfExtents(halfExtents)
    {
        createBody();
    }

    virtual btScalar getMass() override
    {
        return halfExtents.x()*halfExtents.y()*halfExtents.z()*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode() override
    {
        node = device->getSceneManager()->addCubeSceneNode(1, 0, -1,
            core::vector3df(position.x(), position.y(), position.z()));
        node->setScale(core::vector3df(halfExtents.x() * 2, halfExtents.y() * 2, halfExtents.z() * 2));
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));

        // just for cubes' beauty
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    }

    virtual void createMotionState() override
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual void createShape() override
    {
        // create shape for cubes
        shape = new btBoxShape(halfExtents);
    }

private:
    btVector3 halfExtents;
};

#endif // BOX_H
