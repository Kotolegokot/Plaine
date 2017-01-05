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

#include "PlaneProducer.hpp"

using namespace irr;

std::unique_ptr<Body> PlaneProducer::produce(btDynamicsWorld &physicsWorld,
                                             IrrlichtDevice &irrlichtDeivce,
                                             const btVector3 &position) const
{
    btTransform absoluteTransform = relativeTransform;
    absoluteTransform.getOrigin() += position;

    auto node = createNode(irrlichtDeivce, absoluteTransform);
    node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
    auto motionState = std::make_unique<MotionState>(btTransform::getIdentity(), node.release());
    auto shape = createShape();
    btScalar mass = getMass();

    btVector3 inertia(0, 0, 0);
    if (mass)
        shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState.release(),
                                                         shape.release(), inertia);

    auto rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);
    rigidBody->setCenterOfMassTransform(absoluteTransform);
    rigidBody->setUserIndex(1); // default index for bodies
    rigidBody->setActivationState(DISABLE_DEACTIVATION);
    physicsWorld.addRigidBody(rigidBody.get());

    return std::make_unique<Plane>(physicsWorld, std::move(rigidBody));
}

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
