#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace irr;

// this class is a contact place for Bullet and Irrlicht
// it describes an Irrlicht node and a Bullet body at the same time
// Bullet uses its (get|set)WorldTransform functions to
//      change a body's orientation and position,
//      and MotionState changes the orientation and
//      position of the Irrlicht node
// so that it syncs a Bullet body and its correspoindig Irrlicht node
class MotionState : public btMotionState
{
protected:
	btTransform transform;
 	scene::ISceneNode *node = nullptr;
    static btVector3 quatToEuler(const btQuaternion &quat);

public:
	MotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	virtual ~MotionState();
	void setNode(scene::ISceneNode *node);
	scene::ISceneNode *getNode() const;
	void setPosition(const core::vector3df &position);
	core::vector3df getPosition() const;
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif // MOTIONSTATE_H
