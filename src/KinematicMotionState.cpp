#include "KinematicMotionState.h"

using namespace irr;

KinematicMotionState::KinematicMotionState(const btTransform &startTransform, scene::ISceneNode *node) :
    MotionState(startTransform, node) {}

void KinematicMotionState::setWorldTransform(const btTransform &worldTrans)
{
    // nothing, because Bullet has no power here ahahahhaaha
}

void KinematicMotionState::setKinematicWorldTransform(const btTransform &worldTrans)
{
    transform = worldTrans;

    if (node) {
        btQuaternion quatRotation = worldTrans.getRotation();
        btVector3 eulerRotation = quatToEuler(quatRotation);
        node->setRotation(core::vector3df(eulerRotation.x(), eulerRotation.y(), eulerRotation.z()));
        btVector3 pos = worldTrans.getOrigin();
        node->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
    }
}
