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
    node.release()->remove();
}

void MotionState::setNode(std::unique_ptr<scene::ISceneNode> node)
{
    this->node = std::move(node);
}

scene::ISceneNode &MotionState::getNode()
{
    return *node;
}

void MotionState::setPosition(const core::vector3df &position)
{
    transform.setOrigin(irrlicht2bullet(position));
    node->setPosition(position);
}

core::vector3df MotionState::getPosition() const
{
    return bullet2irrlicht(transform.getOrigin());
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
    #if NAN_ASSERT
        assert(!std::isnan(worldTrans.getOrigin().getX()));
        assert(!std::isnan(worldTrans.getOrigin().getY()));
        assert(!std::isnan(worldTrans.getOrigin().getZ()));
        assert(!std::isnan(worldTrans.getRotation().getX()));
        assert(!std::isnan(worldTrans.getRotation().getY()));
        assert(!std::isnan(worldTrans.getRotation().getZ()));
        assert(!std::isnan(worldTrans.getRotation().getW()));
    #endif // NAN_ASSERT

    if (node) {
        node->setRotation(quatToEuler(worldTrans.getRotation()));
        node->setPosition(bullet2irrlicht(worldTrans.getOrigin()));
    }
}
