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

#include "Plane.h"

using namespace irr;

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world, device, position)
{
    createBody();
    //rigidBody->setAngularFactor(btVector3(0, 0, 1));

    world->setInternalTickCallback(
        [](btDynamicsWorld *world, btScalar timeStep)
        {
            Plane *plane = static_cast<Plane *>(world->getWorldUserInfo());
            btVector3 aV = plane->getRigidBody().getAngularVelocity();
            btScalar aVLength = aV.length();
            if (aVLength > 0) {
                aV.safeNormalize();

                aV *= aVLength - 0.1f*aVLength*aVLength;

                plane->getRigidBody().setAngularVelocity(aV);
            }

        }, static_cast<void *>(this));
}

std::unique_ptr<scene::ISceneNode> Plane::createNode()
{
    std::unique_ptr<scene::IMesh> mesh(device->getSceneManager()->getMesh(PLANE_MODEL));

    std::unique_ptr<scene::ISceneNode> node(device->getSceneManager()->addMeshSceneNode(mesh.release()));
    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/plane.png"));

    return node;
}

void Plane::createMotionState(std::unique_ptr<scene::ISceneNode> node)
{
    motionState = std::make_unique<MotionState>(btTransform(btQuaternion(0, 0, 0, 1), position), node.release());
}

void Plane::createShape()
{
    ObjMesh objMesh(PLANE_MODEL);

    shape = std::make_unique<btConvexTriangleMeshShape>(objMesh.getTriangleMesh());
}

btScalar Plane::getMass()
{
    return 1;
}

btVector3 Plane::getLinearVelocity() const
{
    return rigidBody->getLinearVelocity();
}

void Plane::setLinearVelocity(const btVector3 &linearVelocity)
{
    rigidBody->setLinearVelocity(linearVelocity);
}

btScalar Plane::getScalarLinearVelocity() const
{
    return rigidBody->getLinearVelocity().length();
}

void Plane::setScalarLinearVelocity(btScalar length)
{
    btVector3 linearVelocity = rigidBody->getLinearVelocity();
    linearVelocity.safeNormalize();

    rigidBody->setLinearVelocity(linearVelocity * length);
}

btVector3 Plane::getAngularVelocity() const
{
    return rigidBody->getAngularVelocity();
}

void Plane::setAngularVelocity(const btVector3 &angularVelocity)
{
    rigidBody->setAngularVelocity(angularVelocity);
}

btScalar Plane::getScalarAngularVelocity() const
{
    return rigidBody->getAngularVelocity().length();
}

void Plane::setScalarAngularVelocity(btScalar length)
{
    btVector3 angularVelocity = rigidBody->getAngularVelocity();
    angularVelocity.safeNormalize();

    rigidBody->setAngularVelocity(angularVelocity * length);
}

btQuaternion Plane::getRotation() const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);

    return transform.getRotation();
}

void Plane::setRotation(const btQuaternion &rotation)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    transform.setRotation(rotation);

    rigidBody->getMotionState()->setWorldTransform(transform);
}

btVector3 Plane::getEulerRotation() const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion bulletQuaternion = transform.getRotation();

    core::quaternion irrlichtQuaternion(bulletQuaternion.x(), bulletQuaternion.y(),
        bulletQuaternion.z(), bulletQuaternion.w());
    core::vector3df eulerRotation;
    irrlichtQuaternion.toEuler(eulerRotation);

    return btVector3(eulerRotation.X, eulerRotation.Y, eulerRotation.Z);
}

void Plane::setEulerRotation(const btVector3 &rotation)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion quaternionRotation(rotation.x(), rotation.y(), rotation.z());
    transform.setRotation(quaternionRotation);

    rigidBody->getMotionState()->setWorldTransform(transform);
}

void Plane::getAxisAngleRotation(btVector3 &axis, btScalar &angle) const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion rotation = transform.getRotation();

    axis = rotation.getAxis();
    angle = rotation.getAngle();
}

void Plane::setAxisAngleRotation(const btVector3 &axis, btScalar angle)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion rotation(axis, angle);
    transform.setRotation(rotation);

    rigidBody->getMotionState()->setWorldTransform(transform);
}
