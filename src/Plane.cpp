#include "Plane.h"

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world), device(device), position(position)
{
    createBody();
    //rigidBody->setAngularFactor(btVector3(0, 0, 1));

    world->setInternalTickCallback(
        [](btDynamicsWorld *world, btScalar timeStep)
        {
            Plane *plane = static_cast<Plane *>(world->getWorldUserInfo());

            btVector3 aV = plane->getRigidBody()->getAngularVelocity();
            btScalar aVLength = aV.length();
            if (aVLength > 0) {
                aV.safeNormalize();

                if (aVLength < 0.04f)
                    aV *= 0;
                else
                    aV *= aVLength - 0.04f;

                plane->getRigidBody()->setAngularVelocity(aV);
            }

        }, static_cast<void *>(this));
}

void Plane::createNode()
{
    planeMesh = device->getSceneManager()->getMesh(PLANE_MODEL);
    node = device->getSceneManager()->addMeshSceneNode(planeMesh);
    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/plane.png"));
}

void Plane::createMotionState()
{
    motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
}

void Plane::createShape()
{
    ObjMesh objMesh;
    objMesh.loadMesh(PLANE_MODEL);

    shape = new btConvexTriangleMeshShape(objMesh.getTriangleMesh());
}

btScalar Plane::getMass()
{
        return 1;
}

const btVector3 &Plane::getLinearVelocity() const
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

const btVector3 &Plane::getAngularVelocity() const
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

const btVector3 &Plane::getPosition() const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);

    return transform.getOrigin();
}

void Plane::setPosition(const btVector3 &position)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(position);

    rigidBody->getMotionState()->setWorldTransform(transform);
}
