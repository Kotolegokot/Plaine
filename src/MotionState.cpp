/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MotionState.h"

using namespace irr;

MotionState::MotionState(const btTransform &startTransform, scene::ISceneNode *node) :
    transform(startTransform), node(node) {}

MotionState::~MotionState()
{
    node->remove();
}

void MotionState::setNode(scene::ISceneNode *node)
{
    this->node = node;
}

scene::ISceneNode *MotionState::getNode() const
{
    return node;
}

void MotionState::setPosition(const core::vector3df &position)
{
    transform.setOrigin(btVector3(position.X, position.Y, position.Z));
    node->setPosition(position);
}

core::vector3df MotionState::getPosition() const
{
    btVector3 origin = transform.getOrigin();

    return core::vector3df(origin.x(), origin.y(), origin.z());
}

// gets the body's transformation
void MotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans = transform;
}

// sets the body's and node's transformation
// to set the Irrlicht node's rotation it converts
//      quaternion notation to Euler angles
void MotionState::setWorldTransform(const btTransform &worldTrans)
{
    transform = worldTrans;

    if (node) {
        // rotation
        core::vector3df eulerRotation;
        btQuaternion quatRotation;
        worldTrans.getBasis().getRotation(quatRotation);
        core::quaternion q(quatRotation.getX(), quatRotation.getY(), quatRotation.getZ(), quatRotation.getW());
        q.toEuler(eulerRotation);
        eulerRotation *= core::RADTODEG;
        node->setRotation(eulerRotation);

        // position
        btVector3 pos = worldTrans.getOrigin();
        node->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
    }
}
