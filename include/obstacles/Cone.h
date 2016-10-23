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

#ifndef CONE_H
#define CONE_H

#include "ObjMesh.h"
#include "IBody.h"

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define CONE_MODEL "media/models/cone.obj"

class Cone : public IBody
{
public:
    Cone(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position, btScalar radius, btScalar height) :
        IBody(world, device, position), radius(radius), height(height)
    {
        createBody();
    }

    virtual btScalar getMass() override
    {
        btScalar k = core::PI / 3.0f;
        return height*radius*radius*k*MASS_COEFFICIENT;
    }

protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode() override
    {
        std::unique_ptr<scene::IMesh> mesh(device.getSceneManager()->getMesh(CONE_MODEL));

        std::unique_ptr<scene::ISceneNode> node(device.getSceneManager()->addMeshSceneNode(mesh.release()));
        node->setScale(core::vector3df(radius * 2, height, radius * 2));
        node->setMaterialTexture(0, device.getVideoDriver()->getTexture("media/textures/4.png"));
        node->setVisible(TEXTURES_ENABLED);
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
        if (!objMesh)
            objMesh = new ObjMesh(CONE_MODEL);

        shape = std::make_unique<btConvexHullShape>();
        objMesh->setPoints((btConvexHullShape &) *shape);
        shape->setLocalScaling(btVector3(radius * 2, height, radius * 2));
    }


private:
    btScalar radius;
    btScalar height;

    static ObjMesh *objMesh;
};

#endif // TETRAHEDRON_H
