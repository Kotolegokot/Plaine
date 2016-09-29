#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

SceneNodeAnimatorCameraPlayer::SceneNodeAnimatorCameraPlayer(KinematicMotionState *motionState, f32 forwardMoveSpeed,
    f32 lateralMoveSpeed, f32 verticalMoveSpeed, const struct Controls &controls) :
        motionState(motionState),
        forwardMoveSpeed(forwardMoveSpeed),
        lateralMoveSpeed(lateralMoveSpeed),
        verticalMoveSpeed(verticalMoveSpeed),
        controls(controls)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
        PressedKeys[i] = false;
}

f32 SceneNodeAnimatorCameraPlayer::getForwardMoveSpeed() const
{
    return forwardMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setForwardMoveSpeed(f32 moveSpeed)
{
    forwardMoveSpeed = moveSpeed;
}

f32 SceneNodeAnimatorCameraPlayer::getLateralMoveSpeed() const
{
    return lateralMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setLateralMoveSpeed(f32 moveSpeed)
{
    lateralMoveSpeed = moveSpeed;
}

f32 SceneNodeAnimatorCameraPlayer::getVerticalMoveSpeed() const
{
    return verticalMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setVerticalMoveSpeed(f32 moveSpeed)
{
    verticalMoveSpeed = moveSpeed;
}

KinematicMotionState *SceneNodeAnimatorCameraPlayer::getMotionState() const
{
    return motionState;
}

void SceneNodeAnimatorCameraPlayer::setMotionState(KinematicMotionState *motionState)
{
    this->motionState = motionState;
}

bool SceneNodeAnimatorCameraPlayer::OnEvent(const SEvent &event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT)
        PressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
}

void SceneNodeAnimatorCameraPlayer::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    if (!node)
        return;

    const f32 MAX_ACCELERATION = 4;

    if (firstUpdate) {
        lastAnimationTime = timeMs;
        firstUpdate = false;
    }

    /*if (!node->isInputReceiverEnabled()) {
        lastAnimationTime = timeMs;
        return;
    }*/

    scene::ISceneManager * smgr = node->getSceneManager();

    f32 timeDiff = (f32) (timeMs - lastAnimationTime);
    lastAnimationTime = timeMs;

    core::vector3df pos = node->getPosition();

    core::vector3df dir(0, 0, 1);
    core::vector3df up(0, 1, 0);
    core::vector3df left(1, 0, 0);

    // lateral movement
    /*if (PressedKeys[controls.left] && PressedKeys[controls.right]){
        // nothing
    } else if (PressedKeys[controls.left]) {
        if (lateralAcceleration < MAX_ACCELERATION)
            lateralAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 6;
    } else if (PressedKeys[controls.right]) {
        if (lateralAcceleration > -MAX_ACCELERATION)
            lateralAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 6;
    }
    pos += left * timeDiff * lateralMoveSpeed * 0.01f * lateralAcceleration;
    if (lateralAcceleration > 0)
        lateralAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 10;
    else if (lateralAcceleration < 0)
        lateralAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 10;

    // vertical movement
    if (PressedKeys[controls.up] && PressedKeys[controls.down]) {
        // nothing
    } else if (PressedKeys[controls.up]) {
        if (verticalAcceleration < MAX_ACCELERATION)
            verticalAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 6;
    } else if (PressedKeys[controls.down]) {
        if (verticalAcceleration > -MAX_ACCELERATION)
            verticalAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 6;
    }
    pos += up * timeDiff * verticalMoveSpeed * 0.01f * verticalAcceleration;
    if (verticalAcceleration > 0)
        verticalAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 10;
    else if (verticalAcceleration < 0)
        verticalAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 10;*/

    pos += dir * timeDiff * forwardMoveSpeed * 0.01f;

    // apply all the changes
    if (motionState)
        motionState->setPosition(pos);
}

scene::ISceneNodeAnimator *SceneNodeAnimatorCameraPlayer::createClone(scene::ISceneNode *node,
				scene::ISceneManager *newManager)
{
    SceneNodeAnimatorCameraPlayer *newAnimator = new SceneNodeAnimatorCameraPlayer(motionState,
        forwardMoveSpeed, lateralMoveSpeed, verticalMoveSpeed, controls);

	return newAnimator;
}

bool SceneNodeAnimatorCameraPlayer::isEventReceiverEnabled() const
{
    return true;
}
