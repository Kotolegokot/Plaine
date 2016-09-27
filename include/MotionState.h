#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace irr;

class MotionState : public btMotionState
{
protected:
	btTransform transform;
 	scene::ISceneNode *node = nullptr;

public:
	MotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	void setNode(scene::ISceneNode *node);
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);

private:
    static btVector3 quatToEuler(const btQuaternion &quat);
};

#endif // MOTIONSTATE_H
