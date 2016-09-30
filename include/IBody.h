#ifndef IBODY_H
#define IBODY_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "MotionState.h"

class IBody
{
public:
    IBody(btDynamicsWorld *world, btCollisionShape *shape) :
        world(world), shape(shape)
    {}

    virtual ~IBody()
    {
        world->removeRigidBody(rigidBody);
        delete motionState;
        delete rigidBody;
    }

    btRigidBody *getRigidBody()
    {
        return rigidBody;
    }


protected:
    virtual void createNode() = 0;
    virtual void createMotionState() = 0;
    virtual btScalar getMass() = 0;

    // this method must be called in a derived class' constructor
    void createBody()
    {
        createNode();
        createMotionState();
        btScalar mass = getMass();

        btVector3 inertia(0, 0, 0);
        if (mass)
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, inertia);

        rigidBody = new btRigidBody(rigidBodyCI);
        world->addRigidBody(rigidBody);
    }

    // got from outside, must not be deleted
    btDynamicsWorld *world = nullptr;
    btCollisionShape *shape = nullptr;

    // created inside, must be deleted in desctructor
    btMotionState *motionState = nullptr;
    scene::ISceneNode *node = nullptr;
    btRigidBody *rigidBody = nullptr;

};

#endif // IBODY_H
