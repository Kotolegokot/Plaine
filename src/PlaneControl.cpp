#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    btVector3 axis;
    btScalar angle;
    plane->getAxisAngleRotation(axis, angle);

    btVector3 turnImpulse(0, 0, 0);
    btVector3 rollImpulse(0, 0, 0);
    btVector3 pitchImpulse(0, 0, 0);
    btVector3 yawImpulse(0, 0, 0);

    // up
    if (eventReceiver->IsKeyDown(controls.up)) {
        turnImpulse += btVector3(0, 1, 0);
        pitchImpulse += btVector3(-1, 0, 0);
    }
        // down
    if (eventReceiver->IsKeyDown(controls.down)) {
        turnImpulse += btVector3(0, -1, 0);
        pitchImpulse += btVector3(1, 0, 0);
    }
        // left
    if (eventReceiver->IsKeyDown(controls.left)) {
        turnImpulse += btVector3(-1, 0, 0);
        yawImpulse += btVector3(0, -1, 0);
    }
        // right
    if (eventReceiver->IsKeyDown(controls.right)) {
        turnImpulse += btVector3(1, 0, 0);
        yawImpulse += btVector3(0, 1, 0);
    }

    // counterclockwise roll
    if (eventReceiver->IsKeyDown(controls.ccwRoll)) {
            rollImpulse += btVector3(0, 0, 1);
    }
    // clockwise roll
    else if (eventReceiver->IsKeyDown(controls.cwRoll)) {
            rollImpulse += btVector3(0, 0, -1);
    }

    // air resistance simulation
    btVector3 planeVelosity = -plane->getRigidBody()->getLinearVelocity();
    btScalar planeVelocityLength = planeVelosity.length();
    planeVelosity.safeNormalize();
    planeVelosity *= 0.001f*planeVelocityLength*planeVelocityLength;
    plane->getRigidBody()->applyForce(planeVelosity, btVector3(0, 0, 0));

    turnImpulse *= 25;
    rollImpulse *= 150;
    std::cout << "X:" << plane->getEulerRotation().getX() << " Y:" << plane->getEulerRotation().getY() << " Z:" << plane->getEulerRotation().getZ() << std::endl;
    if ((yawImpulse.getY() == 1) && (plane->getEulerRotation().getY() < 0.15f))
        yawImpulse *= 200;
    else if ((yawImpulse.getY() == -1) && (plane->getEulerRotation().getY() > -0.15f))
        yawImpulse *= 200;
    else if ((yawImpulse.getY() == 0) && (plane->getEulerRotation().getY() > 0.0f))
    {
        yawImpulse = btVector3(0, -1, 0);
        yawImpulse *= fabs(plane->getEulerRotation().getY())*fabs(plane->getEulerRotation().getY())*2750;
    }
    else if ((yawImpulse.getY() == 0) && (plane->getEulerRotation().getY() < 0.0f))
    {
        yawImpulse += btVector3(0, 1, 0);
        yawImpulse *= fabs(plane->getEulerRotation().getY())*fabs(plane->getEulerRotation().getY())*2750;
    }
    if ((pitchImpulse.getX() == 1) && (plane->getEulerRotation().getX() < 0.15f))
        pitchImpulse *= 100;
    else if ((pitchImpulse.getX() == -1) && (plane->getEulerRotation().getX() > -0.15f))
        pitchImpulse *= 100;
    else if ((pitchImpulse.getX() == 0) && (plane->getEulerRotation().getX() > 0.0f))
    {
        pitchImpulse = btVector3(-1, 0, 0);
        pitchImpulse *= fabs(plane->getEulerRotation().getX())*fabs(plane->getEulerRotation().getX())*2750;
    }
    else if ((pitchImpulse.getX() == 0) && (plane->getEulerRotation().getX() < 0.0f))
    {
        pitchImpulse = btVector3(1, 0, 0);
        pitchImpulse *= fabs(plane->getEulerRotation().getX())*fabs(plane->getEulerRotation().getX())*2750;
    }
    turnImpulse = turnImpulse.rotate(axis, angle);
    yawImpulse = yawImpulse.rotate(axis, angle);
    pitchImpulse = pitchImpulse.rotate(axis, angle);
    plane->getRigidBody()->applyCentralImpulse(turnImpulse);
    plane->getRigidBody()->applyTorqueImpulse(rollImpulse);
    plane->getRigidBody()->applyTorqueImpulse(pitchImpulse);
    plane->getRigidBody()->applyTorqueImpulse(yawImpulse);
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
