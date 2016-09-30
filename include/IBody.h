#ifndef BODY_H
#define BODY_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "MotionState.h"

virtual class IBody
{
public:
    Body() = default;
    virtual ~Body()
    {
        world->removeRigidBody(rigidBody);
        delete motionState;
        delete rigidBody;
        node->remove();
    }

protected:
    // got from outside, must not be deleted
    btShape *shape = nullptr;
    btWorld *world = nullptr;

    // created inside, must be deleted in desctructor
    btMotionState *motionState = nullptr;
    scene::ISceneNode *node = nullptr;
    btRigidBody *rigidBody = nullptr;

};

#endif // BODY_H
