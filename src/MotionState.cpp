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

void MotionState::setPosition(const core::vector3df &position)
{
    transform.setOrigin(btVector3(position.X, position.Y, position.Z));
    node->setPosition(position);
}

core::vector3df MotionState::getPosition() const
{
    btVector3 origin = transform.getOrigin();

    return core::vector3df(origin.x(), origin.y(), origin.z());
}

// gets the body's transformation
void MotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans = transform;
}

// sets the body's and node's transformation
// to set the Irrlicht node's rotation it converts
//      quaternion notation to Euler angles
void MotionState::setWorldTransform(const btTransform &worldTrans)
{
    transform = worldTrans;

    if (node) {
        // rotation
        core::vector3df eulerRotation;
        const btQuaternion &quatRotation = worldTrans.getRotation();
        core::quaternion q(quatRotation.x(), quatRotation.y(), quatRotation.z(), quatRotation.w());
        q.toEuler(eulerRotation);
        eulerRotation *= core::RADTODEG;
        node->setRotation(eulerRotation);;

        // position
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
