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

#ifndef BOX_H
#define BOX_H

#include "IBody.h"

#define MASS_COEFFICIENT 0.000002

using namespace irr;

class Box : public IBody
{
public:
    Box(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position, const btVector3 &halfExtents) :
        IBody(world, device, position), halfExtents(halfExtents)
    {
        createBody();
    }

    virtual btScalar getMass() override
    {
        return halfExtents.x()*halfExtents.y()*halfExtents.z()*MASS_COEFFICIENT;
    }

protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode() override
    {
        std::unique_ptr<scene::ISceneNode> node(device.getSceneManager()->addCubeSceneNode(1, 0, -1,
            core::vector3df(position.x(), position.y(), position.z())));
        node->setScale(core::vector3df(halfExtents.x() * 2, halfExtents.y() * 2, halfExtents.z() * 2));
        node->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/lsd.png"));
        node->setVisible(TEXTURES_ENABLED);
        // just for cubes' beauty
        if (FOG_ENABLED)
            node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

        return node;
    }

    virtual void createMotionState(std::unique_ptr<scene::ISceneNode> node) override
    {
        motionState = std::make_unique<MotionState>(btTransform(btQuaternion(0, 0, 0, 1), position), node.release());
    }

    virtual void createShape() override
    {
        // create shape for cubes
        shape = std::make_unique<btBoxShape>(halfExtents);
    }

private:
    btVector3 halfExtents;
};

#endif // BOX_H
