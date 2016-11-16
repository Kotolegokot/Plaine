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

#ifndef ICOSPHERE2_H
#define ICOSPHERE2_H

#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "ObjMesh.h"
#include "IBody.h"
#include "util.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define ICOSPHERE2_MODEL "media/models/icosphere2.obj"

class Icosphere2 : public IBody
{
public:
    Icosphere2(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position, btScalar radius) :
        IBody(world, device, position), radius(radius)
    {
        createBody();
    }

    btScalar getMass() override
    {
        static constexpr btScalar k = 4.0f / 3.0f * PI<btScalar>;
        return radius*radius*radius*k*MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode() override
    {
        std::unique_ptr<scene::IMesh> mesh(device.getSceneManager()->getMesh(ICOSPHERE2_MODEL));

        std::unique_ptr<scene::ISceneNode> node(device.getSceneManager()->addMeshSceneNode(mesh.release()));
        node->setScale(core::vector3df(radius, radius, radius) * 2);
        node->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/icosphere2.png"));
        node->setVisible(TEXTURES_ENABLED);
        if (FOG_ENABLED)
            node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

        return node;
    }

    std::unique_ptr<btMotionState>
        createMotionState(std::unique_ptr<scene::ISceneNode> node) override
    {
        return std::make_unique<MotionState>(btTransform(btQuaternion(0, 0, 0, 1), position), node.release());
    }

    std::unique_ptr<btCollisionShape> createShape() override
    {
        return std::make_unique<btConvexPointCloudShape>(objMesh->getPoints(), objMesh->getPointsCount(),
                                                         btVector3(radius, radius, radius) * 2);
    }


private:
    btScalar radius;

    static ObjMesh *objMesh;
};

#endif // ICOSPHERE2_H
