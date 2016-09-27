#ifndef KINEMATICMOTIONSTATE_H
#define KINEMATICMOTIONSTATE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace irr;

class KinematicMotionState : public btMotionState
{
protected:
	btTransform transform;
 	scene::ISceneNode *node = nullptr;

public:
    KinematicMotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	~KinematicMotionState();
	void setNode(scene::ISceneNode *node);
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans); // does nothing
	void setKinematicWorldTransform(const btTransform &worldTrans);

private:
    static btVector3 quatToEuler(const btQuaternion &quat);
};

#endif // KINEMATICMOTIONSTATE_H
