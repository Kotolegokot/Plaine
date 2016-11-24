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

#ifndef BODY_H
#define BODY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "MotionState.h"
#include "util.h"

using namespace irr;

class Body {
public:
    Body(btDynamicsWorld &physicsWorld, std::unique_ptr<btRigidBody> rigidBody) :
        m_physicsWorld(physicsWorld), m_rigidBody(std::move(rigidBody)) {}

    Body(Body &&other) :
        m_physicsWorld(other.m_physicsWorld),
        m_rigidBody(std::move(other.m_rigidBody)) {}

    Body &operator =(Body &&other)
    {
        m_physicsWorld = other.m_physicsWorld;
        m_rigidBody = std::move(other.m_rigidBody);

        return *this;
    }

    Body(const Body &) = delete;
    Body &operator =(const Body &) = delete;

    virtual ~Body();

    btRigidBody &rigidBody() { return *m_rigidBody; }
    const btRigidBody &rigidBody() const { return *m_rigidBody; }

    scene::ISceneNode &node();
    const scene::ISceneNode &node() const;

    btVector3 getPosition() const;
    void setPosition(const btVector3 &position);

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

    btVector3 getEulerRotationRad() const;
    btVector3 getEulerRotationDeg() const;
    void setEulerRotationRad(const btVector3 &rotation);
    void setEulerRotationDeg(const btVector3 &rotation);

    void getAxisAngleRotation(btVector3 &axis, btScalar &angle) const;
    void setAxisAngleRotation(const btVector3 &axis, btScalar angle);

protected:
    btDynamicsWorld &m_physicsWorld;
    std::unique_ptr<btRigidBody> m_rigidBody;
};

#endif // BODY_H
