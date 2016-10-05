#ifndef CUBE_H
#define CUBE_H

#include "IBody.h"

#define MASS_COEFFICIENT 0.000002

using namespace irr;

class Cube : public IBody
{
public:
    Cube(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, const f32 &side) :
        IBody(world), device(device), position(position), side(side)
    {
        createBody();
    }

    virtual btScalar getMass() override
    {
        return side*side*side*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode() override
    {
        node = device->getSceneManager()->addCubeSceneNode(side, 0, -1,
            core::vector3df(position.x(), position.y(), position.z()));
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
        shape = new btBoxShape(btVector3(side/2, side/2, side/2));
    }

private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
    f32 side;
};
#endif // CUBE_H
