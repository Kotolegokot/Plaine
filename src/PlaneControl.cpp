#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    btVector3 turnForce(0, 0, 1);

    // up and down
    if (!(eventReceiver->IsKeyDown(controls.up) &&
        eventReceiver->IsKeyDown(controls.down))) { // if not both are pressed
        // up
        if (eventReceiver->IsKeyDown(controls.up)) {
            turnForce += btVector3(0, 50, 0);
        }
        // down
        else if (eventReceiver->IsKeyDown(controls.down)) {
            turnForce += btVector3(0, -50, 0);
        }
    }

    // left and right
    if (!(eventReceiver->IsKeyDown(controls.left) &&
        eventReceiver->IsKeyDown(controls.right))) { // if not both are pressed
        // left
        if (eventReceiver->IsKeyDown(controls.left)) {
            turnForce += btVector3(-50, 0, 0);
        }
        // right
        else if (eventReceiver->IsKeyDown(controls.right)) {
            turnForce += btVector3(50, 0, 0);
        }
    }

    // air resistance simulation
    btVector3 planeVelosity = -plane->getRigidBody()->getLinearVelocity();
    btScalar planeVelocityLength = planeVelosity.length();
    planeVelosity.safeNormalize();
    planeVelosity *= 0.001f*planeVelocityLength*planeVelocityLength;
    plane->getRigidBody()->applyForce(planeVelosity, btVector3(0, 0, 0));

    turnForce.normalize();
    turnForce *= 1000;
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
