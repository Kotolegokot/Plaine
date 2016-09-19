#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

SceneNodeAnimatorCameraPlayer::SceneNodeAnimatorCameraPlayer(f32 straightMoveSpeed, f32 lateralMoveSpeed) :
    straightMoveSpeed(straightMoveSpeed), lateralMoveSpeed(lateralMoveSpeed)
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
    if (!node || node->getType() != scene::ESNT_CAMERA)
		return;

    scene::ICameraSceneNode *camera = static_cast<scene::ICameraSceneNode *>(node);
    if (firstUpdate)
        lastAnimationTime = timeMs;

    if (!camera->isInputReceiverEnabled())
        return;

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
    left.rotateXZBy(90);

    //movedir * timeDiff * MoveSpeed;
    if (PressedKeys[KEY_KEY_A])
        pos += left * timeDiff * lateralMoveSpeed;
    else if (PressedKeys[KEY_KEY_D])
        pos -= left * timeDiff * lateralMoveSpeed;

    if (PressedKeys[KEY_SPACE])
        pos += up * timeDiff * lateralMoveSpeed;
    else if (PressedKeys[KEY_LSHIFT])
        pos -= up * timeDiff * lateralMoveSpeed;

    //pos += dir * timeDiff * straightMoveSpeed;

    camera->setPosition(pos);
}

scene::ISceneNodeAnimator *SceneNodeAnimatorCameraPlayer::createClone(scene::ISceneNode *node,
				scene::ISceneManager *newManager)
{
    SceneNodeAnimatorCameraPlayer *newAnimator = new SceneNodeAnimatorCameraPlayer(straightMoveSpeed, lateralMoveSpeed);

	return newAnimator;
}
