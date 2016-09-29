#include "MotionState.h"

using namespace irr;

MotionState::MotionState(const btTransform &startTransform, scene::ISceneNode *node) :
    transform(startTransform), node(node) {}

MotionState::~MotionState()
{
    node->remove();
}

void MotionState::setNode(scene::ISceneNode *node)
{
    this->node = node;
}

scene::ISceneNode *MotionState::getNode() const
{
    return node;
}

void MotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans = transform;
}

void MotionState::setWorldTransform(const btTransform &worldTrans)
{
    transform = worldTrans;

    if (node) {
        btQuaternion quatRotation = worldTrans.getRotation();
        core::quaternion irrQuatRotation(quatRotation.x(), quatRotation.y(), quatRotation.z(), quatRotation.w());
        node->setRotationQ(irrQuatRotation);
        btVector3 pos = worldTrans.getOrigin();
        node->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
    }
}

btVector3 MotionState::quatToEuler(const btQuaternion &quat)
{
    btVector3 result;
    btScalar q0 = quat.w(), q1 = quat.x(),
        q2 = quat.y(), q3 = quat.z();

    result.setX(atan2(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 + q2*q2)));
    result.setY(asin(2*(q0*q2 - q3*q1)));
    result.setZ(atan2(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 + q3*q3)));

    return result;
}
