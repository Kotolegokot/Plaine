#ifndef PLANECONTROL_H
#define PLANECONTROL_H

#include "EventReceiver.h"
#include "Config.h"
#include "Plane.h"
#include "util.h"

class PlaneControl
{
public:
    PlaneControl(Plane *plane, const Controls &controls = Controls());
    void handle(EventReceiver *eventReceiver);
    void setPlane(Plane *plane);
    Plane *getPlane();

private:
    Plane *plane = nullptr;
    const Controls &controls;
};

#endif // PLANECONTROL_H
