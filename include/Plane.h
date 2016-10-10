#ifndef PLANE_H
#define PLANE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IBody.h"
#include "ObjMesh.h"

#define PLANE_MODEL "media/models/plane.obj"
#define CAMERA_DISTANCE 200
#define PLANE_MASS 1

// this class defines plane
// currently it is just a sphere
//
// see IBody for more information
class Plane : public IBody
{
public:
    Plane(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position);

    // some convenient
    btVector3 getLinearVelocity() const;
    void setLinearVelocity(const btVector3 &linearVelocity);

    btScalar getScalarLinearVelocity() const;
    void setScalarLinearVelocity(btScalar length);

    btVector3 getAngularVelocity() const;
    void setAngularVelocity(const btVector3 &angularVelocity);

    btScalar getScalarAngularVelocity() const;
    void setScalarAngularVelocity(btScalar length);

    btQuaternion getRotation() const;
    void setRotation(const btQuaternion &rotation);

    btVector3 getEulerRotation() const;
    void setEulerRotation(const btVector3 &rotation);

    void getAxisAngleRotation(btVector3 &axis, btScalar &angle) const;
    void setAxisAngleRotation(const btVector3 &axis, btScalar angle);

protected:
    virtual void createNode() override;
    virtual void createMotionState() override;
    virtual void createShape() override;
    virtual btScalar getMass() override;

private:
    scene::IMesh *planeMesh = nullptr;
};

#endif // PLANE_H
