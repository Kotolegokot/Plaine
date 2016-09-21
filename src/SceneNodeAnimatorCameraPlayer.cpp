#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

SceneNodeAnimatorCameraPlayer::SceneNodeAnimatorCameraPlayer(f32 straightMoveSpeed, f32 lateralMoveSpeed, f32 verticalMoveSpeed) :
    straightMoveSpeed(straightMoveSpeed), lateralMoveSpeed(lateralMoveSpeed), verticalMoveSpeed(verticalMoveSpeed)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
        PressedKeys[i] = false;
}

f32 SceneNodeAnimatorCameraPlayer::getStraightMoveSpeed() const
{
    return straightMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setStraightMoveSpeed(f32 moveSpeed)
{
    if (moveSpeed >= 0)
        straightMoveSpeed = moveSpeed;
}

f32 SceneNodeAnimatorCameraPlayer::getLateralMoveSpeed() const
{
    return lateralMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setLateralMoveSpeed(f32 moveSpeed)
{
    if (moveSpeed >= 0)
        lateralMoveSpeed = moveSpeed;
}

f32 SceneNodeAnimatorCameraPlayer::getVerticalMoveSpeed() const
{
    return verticalMoveSpeed;
}

void SceneNodeAnimatorCameraPlayer::setVerticalMoveSpeed(f32 moveSpeed)
{
    if (moveSpeed >= 0)
        verticalMoveSpeed = moveSpeed;
}

void SceneNodeAnimatorCameraPlayer::stop()
{
    straightMoveSpeed = 0;
}

bool SceneNodeAnimatorCameraPlayer::OnEvent(const SEvent &event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT)
        PressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
}

void SceneNodeAnimatorCameraPlayer::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    const f32 MAX_ACCELERATION = 5;

    if (!node || node->getType() != scene::ESNT_CAMERA)
		return;

    scene::ICameraSceneNode *camera = static_cast<scene::ICameraSceneNode *>(node);
    if (firstUpdate) {
        lastAnimationTime = timeMs;
        firstUpdate = false;
    }

    if (!camera->isInputReceiverEnabled()) {
        lastAnimationTime = timeMs;
        return;
    }

    scene::ISceneManager * smgr = camera->getSceneManager();
	if(smgr && smgr->getActiveCamera() != camera)
		return;

    f32 timeDiff = (f32) (timeMs - lastAnimationTime);
    lastAnimationTime = timeMs;

    core::vector3df pos = camera->getPosition();

    core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
    dir.normalize();
    core::vector3df up(0, 1, 0);
    core::vector3df left = dir;
    f32 t = left.X;
    left.X = -left.Z;
    left.Z = t;

    // lateral movement
    if (PressedKeys[KEY_KEY_A]) {
        if (lateralAcceleration < MAX_ACCELERATION)
            lateralAcceleration += 0.1;
    } else if (PressedKeys[KEY_KEY_D]) {
        if (lateralAcceleration > -MAX_ACCELERATION)
            lateralAcceleration -= 0.1;
    }
    pos += left * timeDiff * lateralMoveSpeed * 0.01f * lateralAcceleration;
    if (lateralAcceleration > 0)
        lateralAcceleration -= 0.01;
    else if (lateralAcceleration < 0)
        lateralAcceleration += 0.01;

    // vertical movement
    if (PressedKeys[KEY_KEY_W]) {
        if (verticalAcceleration < MAX_ACCELERATION)
            verticalAcceleration += 0.1;
    } else if (PressedKeys[KEY_KEY_S]) {
        if (verticalAcceleration > -MAX_ACCELERATION)
            verticalAcceleration -= 0.1;
    }
    pos += up * timeDiff * verticalMoveSpeed * 0.01f * verticalAcceleration;
    if (verticalAcceleration > 0)
        verticalAcceleration -= 0.01;
    else if (verticalAcceleration < 0)
        verticalAcceleration += 0.01;

    // moving forward
    pos += dir * timeDiff * straightMoveSpeed * 0.05f;

    // apply all the changes
    camera->setTarget(pos + dir);
    camera->setPosition(pos);
}

scene::ISceneNodeAnimator *SceneNodeAnimatorCameraPlayer::createClone(scene::ISceneNode *node,
				scene::ISceneManager *newManager)
{
    SceneNodeAnimatorCameraPlayer *newAnimator = new SceneNodeAnimatorCameraPlayer(straightMoveSpeed, lateralMoveSpeed);

	return newAnimator;
}

bool SceneNodeAnimatorCameraPlayer::isEventReceiverEnabled() const
{
    return true;
}
