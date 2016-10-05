#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    btTransform transform;
    plane->getRigidBody()->getMotionState()->getWorldTransform(transform);
    btQuaternion rotation = transform.getRotation();
    btVector3 axis = rotation.getAxis();
    btScalar angle = rotation.getAngle();

    btVector3 turnImpulse(0, 0, 0);
    btVector3 rotateImpulse(0, 0, 0);

    // up
    if (eventReceiver->IsKeyDown(controls.up)) {
        turnImpulse += btVector3(0, 1, 0);
    }
        // down
    if (eventReceiver->IsKeyDown(controls.down)) {
        turnImpulse += btVector3(0, -1, 0);
    }
        // left
    if (eventReceiver->IsKeyDown(controls.left)) {
        turnImpulse += btVector3(-1, 0, 0);
    }
        // right
    if (eventReceiver->IsKeyDown(controls.right)) {
        turnImpulse += btVector3(1, 0, 0);
    }

    // rotation
    if (!(eventReceiver->IsKeyDown(controls.ccwRoll) &&
        eventReceiver->IsKeyDown(controls.cwRoll))) { // if not both are pressed
        // counterclockwise roll
        if (eventReceiver->IsKeyDown(controls.ccwRoll)) {
            rotateImpulse = btVector3(0, 0, 1);
        }
        // clockwise roll
        else if (eventReceiver->IsKeyDown(controls.cwRoll)) {
            rotateImpulse = btVector3(0, 0, -1);
        }
    }

    // air resistance simulation
    btVector3 planeVelosity = -plane->getRigidBody()->getLinearVelocity();
    btScalar planeVelocityLength = planeVelosity.length();
    planeVelosity.safeNormalize();
    planeVelosity *= 0.001f*planeVelocityLength*planeVelocityLength;
    plane->getRigidBody()->applyForce(planeVelosity, btVector3(0, 0, 0));

   //turnImpulse = turnImpulse.rotate(axis, angle);
    turnImpulse *= 25;
    rotateImpulse *= 150;
    plane->getRigidBody()->applyCentralImpulse(turnImpulse);
    plane->getRigidBody()->applyTorqueImpulse(rotateImpulse);
    plane->getRigidBody()->applyForce(btVector3(0, 0, 600), btVector3(0, 0, 0));
}

void PlaneControl::setPlane(Plane *plane)
{
    this->plane = plane;
}

Plane *PlaneControl::getPlane()
{
    return plane;
}
