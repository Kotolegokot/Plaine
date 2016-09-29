#ifndef KINEMATICMOTIONSTATE_H
#define KINEMATICMOTIONSTATE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <MotionState.h>

using namespace irr;

class KinematicMotionState : public MotionState {

public:
    KinematicMotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	virtual void setWorldTransform(const btTransform &worldTrans); // does nothing
	void setKinematicWorldTransform(const btTransform &worldTrans);
};

#endif // KINEMATICMOTIONSTATE_H
