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

#ifndef IBODY_H
#define IBODY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "IObstacle.h"
#include "MotionState.h"

// this class is a useful interface joining together
// an Irrlicht node and a Bullet body
class IBody : public IObstacle
{
public:
    IBody(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position) :
        IObstacle(world, device, position) {}

    virtual ~IBody()
    {
        world.removeRigidBody(rigidBody.get());
    }

    btRigidBody &getRigidBody()
    {
        return *rigidBody;
    }

    scene::ISceneNode &getNode()
    {
        return *((MotionState *) rigidBody->getMotionState())->getNode();
    }

    btVector3 getPosition() const override
    {
        btTransform transform;
        rigidBody->getMotionState()->getWorldTransform(transform);

        return transform.getOrigin();
    }

    void setPosition(const btVector3 &position) override
    {
        btTransform transform;
        rigidBody->getMotionState()->getWorldTransform(transform);
        transform.setOrigin(position);

        rigidBody->setCenterOfMassTransform(transform);
    }

    virtual btScalar getMass() = 0;

protected:
    virtual std::unique_ptr<scene::ISceneNode> createNode() = 0;
    virtual void createMotionState(std::unique_ptr<scene::ISceneNode> node) = 0;
    virtual void createShape() = 0;

    // this method must be called in a derived class' constructor
    // creates the node and the body
    //      and add the body to the physics (Bullet) world
    void createBody()
    {
        std::unique_ptr<scene::ISceneNode> node = createNode();
        createMotionState(std::move(node));
        createShape();
        btScalar mass = getMass();

        btVector3 inertia(0, 0, 0);
        if (mass)
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState.get(), shape.get(), inertia);

        rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);
        world.addRigidBody(rigidBody.get());
    }

    // created inside, must be deleted in desctructor
    std::unique_ptr<MotionState> motionState;
    std::unique_ptr<btCollisionShape> shape;
    std::unique_ptr<btRigidBody> rigidBody;
};

#endif // IBODY_H
