#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

SceneNodeAnimatorCameraPlayer::SceneNodeAnimatorCameraPlayer(f32 forwardMoveSpeed, f32 lateralMoveSpeed, f32 verticalMoveSpeed) :
    forwardMoveSpeed(forwardMoveSpeed), lateralMoveSpeed(lateralMoveSpeed), verticalMoveSpeed(verticalMoveSpeed)
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

bool SceneNodeAnimatorCameraPlayer::OnEvent(const SEvent &event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT)
        PressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
}

void SceneNodeAnimatorCameraPlayer::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    const f32 MAX_ACCELERATION = 4;

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
    if (PressedKeys[KEY_KEY_A] && PressedKeys[KEY_KEY_D] ||
            PressedKeys[KEY_KEY_H] && PressedKeys[KEY_KEY_L]) {
        // nothing
    } else if (PressedKeys[KEY_KEY_A] || PressedKeys[KEY_KEY_H]) {
        if (lateralAcceleration < MAX_ACCELERATION)
            lateralAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 6;
    } else if (PressedKeys[KEY_KEY_D] || PressedKeys[KEY_KEY_L]) {
        if (lateralAcceleration > -MAX_ACCELERATION)
            lateralAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 6;
    }
    pos += left * timeDiff * lateralMoveSpeed * 0.01f * lateralAcceleration;
    if (lateralAcceleration > 0)
        lateralAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 10;
    else if (lateralAcceleration < 0)
        lateralAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 10;

    // vertical movement
    if (PressedKeys[KEY_KEY_W] && PressedKeys[KEY_KEY_S] ||
            PressedKeys[KEY_KEY_K] && PressedKeys[KEY_KEY_J]) {
        // nothing
    } else if (PressedKeys[KEY_KEY_W] || PressedKeys[KEY_KEY_K]) {
        if (verticalAcceleration < MAX_ACCELERATION)
            verticalAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 6;
    } else if (PressedKeys[KEY_KEY_S] || PressedKeys[KEY_KEY_J]) {
        if (verticalAcceleration > -MAX_ACCELERATION)
            verticalAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 6;
    }
    pos += up * timeDiff * verticalMoveSpeed * 0.01f * verticalAcceleration;
    if (verticalAcceleration > 0)
        verticalAcceleration -= timeDiff * 0.01f * MAX_ACCELERATION / 10;
    else if (verticalAcceleration < 0)
        verticalAcceleration += timeDiff * 0.01f * MAX_ACCELERATION / 10;

    pos += dir * timeDiff * forwardMoveSpeed * 0.01f;

    // apply all the changes
    camera->setTarget(pos + dir);
    camera->setPosition(pos);
}

scene::ISceneNodeAnimator *SceneNodeAnimatorCameraPlayer::createClone(scene::ISceneNode *node,
				scene::ISceneManager *newManager)
{
    SceneNodeAnimatorCameraPlayer *newAnimator = new SceneNodeAnimatorCameraPlayer(lateralMoveSpeed, verticalMoveSpeed);

	return newAnimator;
}

bool SceneNodeAnimatorCameraPlayer::isEventReceiverEnabled() const
{
    return true;
}
