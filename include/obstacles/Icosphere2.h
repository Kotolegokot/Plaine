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

#include "ObjMesh.h"
#include "IBody.h"
#include <cmath>

using namespace irr;

#define MASS_COEFFICIENT 0.000002
#define ICOSPHERE2_MODEL "media/models/icosphere2.obj"

class Icosphere2 : public IBody
{
public:
    Icosphere2(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, btScalar radius) :
        IBody(world, device, position), radius(radius)
    {
        createBody();
    }

    virtual btScalar getMass()
    {
        btScalar k = 4 / 3 * core::PI;
        return radius*radius*radius*k*MASS_COEFFICIENT;
    }

protected:
    virtual void createNode()
    {
        mesh = device->getSceneManager()->getMesh(ICOSPHERE2_MODEL);
        node = device->getSceneManager()->addMeshSceneNode(mesh);
        node->setScale(core::vector3df(radius, radius, radius) * 2);
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));

        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    }

    virtual void createMotionState()
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual void createShape()
    {
        if (!objMesh)
            objMesh = new ObjMesh(ICOSPHERE2_MODEL);

        shape = new btConvexHullShape();
        objMesh->setPoints((btConvexHullShape *) shape);
        shape->setLocalScaling(btVector3(radius, radius, radius) * 2);
    }


private:
    scene::IMesh *mesh = nullptr;
    btScalar radius;

    static ObjMesh *objMesh;
};

#endif // ICOSPHERE2_H
