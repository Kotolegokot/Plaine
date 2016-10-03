#include "PlaneMotionState.h"

PlaneMotionState::PlaneMotionState(const btTransform &startTransform, scene::ISceneNode *node) :
    MotionState(startTransform, node) {}

void PlaneMotionState::setWorldTransform(const btTransform &worldTrans)
{
    transform = worldTrans;

    if (node) {
        // position
        btVector3 pos = worldTrans.getOrigin();
        node->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));

        // no rotation
    }
}
