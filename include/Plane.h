#ifndef PLANE_H
#define PLANE_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IBody.h"
#include "PlaneMotionState.h"
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

protected:
    virtual void createNode() override;
    virtual void createMotionState() override;
    virtual void createShape() override;
    virtual btScalar getMass() override;

private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
    scene::IMesh *planeMesh = nullptr;
};

#endif // PLANE_H
