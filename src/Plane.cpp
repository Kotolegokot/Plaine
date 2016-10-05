#include "Plane.h"

Plane::Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position) :
    IBody(world), device(device), position(position)
{
    createBody();
    rigidBody->setAngularFactor(btVector3(0, 0, 1));

    world->setInternalTickCallback(
        [](btDynamicsWorld *world, btScalar timeStep)
        {
            Plane *plane = static_cast<Plane *>(world->getWorldUserInfo());

            btVector3 aV = plane->getRigidBody()->getAngularVelocity();
            btScalar aVLength = aV.length();
            if (aVLength > 0) {
                aV.safeNormalize();

                if (aVLength < 0.01f)
                    aV *= 0;
                else
                    aV *= aVLength - 0.01f;

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
