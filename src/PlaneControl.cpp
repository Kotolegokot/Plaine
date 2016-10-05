#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    btTransform transform;
    plane->getRigidBody()->getMotionState()->getWorldTransform(transform);
    btVector3 rotation = quatToEuler(transform.getRotation());

    btVector3 turnForce(0, 0, 1);


    // up
    if (eventReceiver->IsKeyDown(controls.up)) {
        turnForce += btVector3(0, 50, 0);
    }
    // down
    if (eventReceiver->IsKeyDown(controls.down)) {
        turnForce += btVector3(0, -50, 0);
    }

    // left
    if (eventReceiver->IsKeyDown(controls.left)) {
        turnForce += btVector3(-50, 0, 0);
    }
    // right
    if (eventReceiver->IsKeyDown(controls.right)) {
        turnForce += btVector3(50, 0, 0);
    }

    // air resistance simulation
    btVector3 planeVelosity = -plane->getRigidBody()->getLinearVelocity();
    btScalar planeVelocityLength = planeVelosity.length();
    planeVelosity.safeNormalize();
    planeVelosity *= 0.001f*planeVelocityLength*planeVelocityLength;
    plane->getRigidBody()->applyForce(planeVelosity, btVector3(0, 0, 0));

    turnForce.normalize();
    turnForce *= 1000;
    turnForce = turnForce.rotate(btVector3(0, 0, 1), rotation.z());
    plane->getRigidBody()->applyForce(turnForce, btVector3(0, 0, 0));
    plane->getRigidBody()->applyForce(btVector3(0, 0, 300), btVector3(0, 0, 0));
}

void PlaneControl::setPlane(Plane *plane)
{
    this->plane = plane;
}

Plane *PlaneControl::getPlane()
{
    return plane;
}
