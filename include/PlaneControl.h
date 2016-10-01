#ifndef PLANECONTROL_H
#define PLANECONTROL_H

#include "Config.h"
#include "Plane.h"

class PlaneControl
{
public:
    PlaneControl(Plane *plane, const Controls &controls = Controls());
    void handle(bool pressedKeys[]);
    void setPlane(Plane *plane);
    Plane *getPlane();

private:
    Plane *plane = nullptr;
    const Controls &controls;
};

#endif // PLANECONTROL_H
