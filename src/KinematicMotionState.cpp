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
    MotionState::setWorldTransform(worldTrans);
}
