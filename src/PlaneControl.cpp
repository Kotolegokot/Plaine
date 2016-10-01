#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    // up and down
    if (!(eventReceiver->IsKeyDown(controls.up) &&
        eventReceiver->IsKeyDown(controls.down))) { // if not both are pressed
        // up
        if (eventReceiver->IsKeyDown(controls.up)) {
            plane->getRigidBody()->applyForce(btVector3(0, 1000, 0), btVector3(0, 0, 0));
        }
        // down
        else if (eventReceiver->IsKeyDown(controls.down)) {
            plane->getRigidBody()->applyForce(btVector3(0, -1000, 0), btVector3(0, 0, 0));
        }
    }

    // left and right
    if (!(eventReceiver->IsKeyDown(controls.left) &&
        eventReceiver->IsKeyDown(controls.right))) { // if not both are pressed
        // left
        if (eventReceiver->IsKeyDown(controls.left)) {
            plane->getRigidBody()->applyForce(btVector3(-1000, 0, 0), btVector3(0, 0, 0));
        }
        // right
        else if (eventReceiver->IsKeyDown(controls.right)) {
            plane->getRigidBody()->applyForce(btVector3(1000, 0, 0), btVector3(0, 0, 0));
        }
    }
}

void PlaneControl::setPlane(Plane *plane)
{
    this->plane = plane;
}

Plane *PlaneControl::getPlane()
{
    return plane;
}
