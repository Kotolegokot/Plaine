#include "KinematicMotionState.h"

using namespace irr;

KinematicMotionState::KinematicMotionState(const btTransform &startTransform, scene::ISceneNode *node) :
    transform(startTransform), node(node) {}

KinematicMotionState::~KinematicMotionState()
{
    node->remove();
}

void KinematicMotionState::setNode(scene::ISceneNode *node)
{
    this->node = node;
}

scene::ISceneNode *KinematicMotionState::getNode() const
{
    return node;
}

void KinematicMotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans = transform;
}

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

btVector3 KinematicMotionState::quatToEuler(const btQuaternion &quat)
{
    btVector3 result;
    btScalar q0 = quat.w(), q1 = quat.x(),
        q2 = quat.y(), q3 = quat.z();

    result.setX(atan(2*(q0*q1 + q2*q3)/(1 - 2*(q1*q1 + q2*q2))));
    result.setY(asin(2*(q0*q2 - q3*q1)));
    result.setZ(atan(2*(q0*q3 + q1*q2)/(1 - 2*(q2*q2 + q3*q3))));

    return result;
}

