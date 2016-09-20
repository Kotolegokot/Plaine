#ifndef SCENENODEANIMATORCAMERAPLAYER_H
#define SCENENODEANIMATORCAMERAPLAYER_H

#include <irrlicht/irrlicht.h>

using namespace irr;


class SceneNodeAnimatorCameraPlayer : public scene::ISceneNodeAnimator
{
public:
    SceneNodeAnimatorCameraPlayer(f32 straightMoveSpeed = 0, f32 lateralMoveSpeed = 0);

    f32 getStraightMoveSpeed() const;
    void setStraightMoveSpeed(f32 moveSpeed);
    f32 getLateralMoveSpeed() const;
    void setLateralMoveSpeed(f32 moveSpeed);
    void stop();

    virtual void animateNode(scene::ISceneNode* node, u32 timeMs) override;
    virtual scene::ISceneNodeAnimator *createClone(scene::ISceneNode *node, scene::ISceneManager *newManager = 0) override;
    virtual bool OnEvent(const SEvent& event) override;
    virtual bool isEventReceiverEnabled() const override;
private:
    f32 straightMoveSpeed = 0;
    f32 lateralMoveSpeed = 0;
    s32 lastAnimationTime;
    bool PressedKeys[KEY_KEY_CODES_COUNT];

    bool firstUpdate = true;
};

#endif // SCENENODEANIMATORCAMERAPLAYER_H
