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

#include "PlaneProducer.h"

using namespace irr;

btScalar PlaneProducer::getMass() const
{
    return 1.0f;
}

std::unique_ptr<scene::ISceneNode> PlaneProducer::createNode(IrrlichtDevice &irrlichtDevice,
                                                     const btTransform &absoluteTransform) const
{
    std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(PLANE_MODEL));

    std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                            addMeshSceneNode(mesh.release()));
    node->setPosition(bullet2irrlicht(absoluteTransform.getOrigin()));
    node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
    node->setMaterialTexture(0, irrlichtDevice.getVideoDriver()->
                             getTexture("media/textures/plane.png"));
    node->setScale({ 15, 15, 15 });

    return node;
}

std::unique_ptr<btCollisionShape> PlaneProducer::createShape() const
{
    ObjMesh objMesh(PLANE_MODEL, 15);

    return std::make_unique<btConvexTriangleMeshShape>(objMesh.getTriangleMesh().release());
}

void PlaneProducer::finishingTouch(btRigidBody &body) const
{
    body.setUserIndex(1);
    body.setUserPointer(&body);
    body.setActivationState(DISABLE_DEACTIVATION);
}
