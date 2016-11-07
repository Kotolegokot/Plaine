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

#ifndef PLANE_H
#define PLANE_H

#include <algorithm>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IBody.h"
#include "ObjMesh.h"
#include "util.h"

#define PLANE_MODEL "media/models/plane.obj"

#if FAR_CAMERA_DISTANCE
    #define CAMERA_DISTANCE 600
#else
    #define CAMERA_DISTANCE 200
#endif // FAR_CAMERA_DISTANCE

#define PLANE_MASS 1

// this class defines plane
// currently it is just a sphere
//
// see IBody for more information
class Plane : public IBody
{
public:
    Plane(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position);

    void setExploded(bool exploded);
    bool getExploded() const;

    void disappear();
    void appear();

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

    long getScore() const;
    void addScore(long addScore);
    void clearScore();

    int getPositiveMultiplier() const;
    void setPositiveMultiplier(int positiveMultiplier);

    int getNegativeMultiplier() const;
    void setNegativeMultiplier(int negativeMultiplier);
protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode() override;
    virtual void createMotionState(std::unique_ptr<scene::ISceneNode> node) override;
    virtual void createShape() override;
    virtual btScalar getMass() override;

private:
    bool exploded = false;
    long score = 0;
    int positiveMultiplier = 1;
    int negativeMultiplier = 1;
};

#endif // PLANE_H
