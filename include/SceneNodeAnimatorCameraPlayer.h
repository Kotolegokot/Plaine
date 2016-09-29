#ifndef SCENENODEANIMATORCAMERAPLAYER_H
#define SCENENODEANIMATORCAMERAPLAYER_H

#include <irrlicht.h>
#include "Config.h"
#include "KinematicMotionState.h"

using namespace irr;


class SceneNodeAnimatorCameraPlayer : public scene::ISceneNodeAnimator
{
public:
    SceneNodeAnimatorCameraPlayer(KinematicMotionState *motionState, f32 forwardMoveSpeed = 0, f32 lateralMoveSpeed = 0, f32 verticalMoveSpeed = 0,
        const struct Controls &controls = Controls());

    f32 getForwardMoveSpeed() const;
    void setForwardMoveSpeed(f32 moveSpeed);
    f32 getLateralMoveSpeed() const;
    void setLateralMoveSpeed(f32 moveSpeed);
    f32 getVerticalMoveSpeed() const;
    void setVerticalMoveSpeed(f32 moveSpeed);

    KinematicMotionState *getMotionState() const;
    void setMotionState(KinematicMotionState *motionState);

    virtual void animateNode(scene::ISceneNode* node, u32 timeMs) override;
    virtual scene::ISceneNodeAnimator *createClone(scene::ISceneNode *node, scene::ISceneManager *newManager = 0) override;
    virtual bool OnEvent(const SEvent& event) override;
    virtual bool isEventReceiverEnabled() const override;
private:
    f32 forwardMoveSpeed = 0;
    f32 lateralMoveSpeed = 0;
    f32 verticalMoveSpeed = 0;
    f32 lateralAcceleration = 0;
    f32 verticalAcceleration = 0;
    s32 lastAnimationTime;

    bool PressedKeys[KEY_KEY_CODES_COUNT];
    bool firstUpdate = true;

    Controls controls;
    KinematicMotionState *motionState = nullptr;
};

#endif // SCENENODEANIMATORCAMERAPLAYER_H
