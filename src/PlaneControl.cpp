#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(bool pressedKeys[])
{
    // up and down
    if (!(pressedKeys[controls.up] && pressedKeys[controls.down])) { // if not both are pressed
        // up
        if (pressedKeys[controls.up]) {

        }
        // down
        else {

        }
    }

    // left and right
    if (!(pressedKeys[controls.left] && pressedKeys[controls.right])) { // if not both are pressed
        // left
        if (pressedKeys[controls.left]) {

        }
        // right
        else {

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
