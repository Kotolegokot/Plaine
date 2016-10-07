#ifndef IBODY_H
#define IBODY_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IObstacle.h"
#include "MotionState.h"

// this class is a useful interface joining together
// an Irrlicht node and a Bullet body
class IBody : public IObstacle
{
public:
    IBody(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
        IObstacle(world, device, position) {}

    virtual ~IBody()
    {
        world->removeRigidBody(rigidBody);
        delete motionState;
        delete shape;
        delete rigidBody;
    }

    btRigidBody *getRigidBody()
    {
        return rigidBody;
    }

    scene::ISceneNode *getNode()
    {
        return node;
    }

    virtual btScalar getMass() = 0;

protected:
    virtual void createNode() = 0;
    virtual void createMotionState() = 0;
    virtual void createShape() = 0;

    // this method must be called in a derived class' constructor
    // creates the node and the body
    //      and add the body to the physics (Bullet) world
    void createBody()
    {
        createNode();
        createMotionState();
        createShape();
        btScalar mass = getMass();

        btVector3 inertia(0, 0, 0);
        if (mass)
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, inertia);

        rigidBody = new btRigidBody(rigidBodyCI);
        world->addRigidBody(rigidBody);
    }

    // created inside, must be deleted in desctructor
    scene::ISceneNode *node = nullptr;
    btMotionState *motionState = nullptr;
    btCollisionShape *shape = nullptr;
    btRigidBody *rigidBody = nullptr;

};

#endif // IBODY_H
