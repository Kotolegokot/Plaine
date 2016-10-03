#ifndef PLANEMOTIONSTATE_H
#define PLANEMOTIONSTATE_H

#include "MotionState.h"

class PlaneMotionState : public MotionState
{
public:
    PlaneMotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif // PLANEMOTIONSTATE_H
