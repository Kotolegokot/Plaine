/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <iostream>
#include <memory>
#include <cassert>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "util/math.h"
#include "util/NaN.h"
#include "util/other.h"
#include "util/options.h"

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
public:
	MotionState(const btTransform &startTransform = btTransform::getIdentity(), scene::ISceneNode *node = nullptr);
	virtual ~MotionState();
	void setNode(std::unique_ptr<scene::ISceneNode> node);
	scene::ISceneNode &getNode();
	void setPosition(const core::vector3df &position);
	core::vector3df getPosition() const;
	virtual void getWorldTransform(btTransform &worldTrans) const;

protected:
    btTransform transform;
    std::unique_ptr<scene::ISceneNode> node;
    virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif // MOTIONSTATE_H
