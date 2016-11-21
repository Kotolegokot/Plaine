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

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <cmath>
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "ObjMesh.h"
#include "IBody.h"
#include "util.h"

using namespace irr;

#define TETRAHEDRON_MODEL "media/models/tetrahedron.obj"

class Tetrahedron : public IBody
{
public:
    Tetrahedron(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position, btScalar edge) :
        IBody(world, device, position), edge(edge)
    {
        createBody();
    }

    btScalar getMass() override
    {
        btScalar k = 1.0f / 6.0f / std::sqrt(2);
        return edge * edge * edge * k * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode() override
    {
        std::unique_ptr<scene::IMesh> mesh(device.getSceneManager()->getMesh(TETRAHEDRON_MODEL));

        std::unique_ptr<scene::ISceneNode> node(device.getSceneManager()->addMeshSceneNode(mesh.release()));
        node->setScale(core::vector3df(edge, edge, edge));
        node->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/tetrahedron.png"));
        node->setVisible(TEXTURES_ENABLED);
        #if FOG_ENABLED
            node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        #endif // FOG_ENABLED
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
                                                         btVector3(edge, edge, edge));
    }

private:
    btScalar edge;

    static ObjMesh *objMesh;
};

#endif // TETRAHEDRON_H
