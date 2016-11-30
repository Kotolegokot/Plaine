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

#include "PlaneControl.h"

constexpr btScalar PlaneControl::FORWARD_IMPULSE;
constexpr btScalar PlaneControl::MAX_PITCH_ANGLE;
constexpr btScalar PlaneControl::MAX_YAW_ANGLE;
constexpr btScalar PlaneControl::MAX_ROLL_VELOCITY;
constexpr btScalar PlaneControl::ROLL_VELOCITY_ACCELERATION_STEP;
constexpr btScalar PlaneControl::ROLL_VELOCITY_DECELERATION_STEP;
constexpr btScalar PlaneControl::LEFT_IMPULSE;
constexpr btScalar PlaneControl::RIGHT_IMPULSE;
constexpr btScalar PlaneControl::UP_IMPULSE;
constexpr btScalar PlaneControl::DOWN_IMPULSE;

PlaneControl::PlaneControl(Plane &plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver &eventReceiver)
{
    btVector3 axis;
    btScalar angle;
    plane.getAxisAngleRotation(axis, angle);

    btVector3 rotation = plane.getEulerRotationRad();

    btVector3 angularVelocity = plane.getAngularVelocity();

    btVector3 sideImpulse(0, 0, 0);

    // turn up and down
    {
        bool up = eventReceiver.isKeyDown(controls[CONTROL::UP]);
        bool down = eventReceiver.isKeyDown(controls[CONTROL::DOWN]);

        if (up && down)
            up = down = false;

        if (up && rotation.x() > -MAX_PITCH_ANGLE && rotation.x() <= PI<btScalar>) {
            angularVelocity.setX(-(rotation.x() + MAX_PITCH_ANGLE) / MAX_PITCH_ANGLE);
        } else if (down && rotation.x() >= -PI<btScalar> && rotation.x() < MAX_PITCH_ANGLE) {
            angularVelocity.setX((-rotation.x() + MAX_PITCH_ANGLE) / MAX_PITCH_ANGLE);
        } else {
            angularVelocity.setX(-rotation.x() / MAX_PITCH_ANGLE);
        }

        if (up)
            sideImpulse += { 0, UP_IMPULSE, 0 };
        else if (down)
            sideImpulse += { 0, -DOWN_IMPULSE, 0 };
    }

    // turn left and right
    {
        bool left = eventReceiver.isKeyDown(controls[CONTROL::LEFT]);
        bool right = eventReceiver.isKeyDown(controls[CONTROL::RIGHT]);

        if (left && right)
            left = right = false;

        if (left && rotation.y() > -MAX_YAW_ANGLE && rotation.y() <= PI<btScalar>)
            angularVelocity.setY(-(rotation.y() + MAX_YAW_ANGLE) / MAX_YAW_ANGLE);
        else if (right && rotation.y() >= -PI<btScalar> && rotation.y() < MAX_YAW_ANGLE)
            angularVelocity.setY((-rotation.y() + MAX_YAW_ANGLE) / MAX_YAW_ANGLE);
        else
            angularVelocity.setY(-rotation.y() / MAX_YAW_ANGLE);

        if (left)
            sideImpulse += { -LEFT_IMPULSE, 0, 0 };
        else if (right)
            sideImpulse += { RIGHT_IMPULSE, 0, 0 };
    }

    // rool ccw and cw
    {
        bool ccw = eventReceiver.isKeyDown(controls[CONTROL::CCW_ROLL]);
        bool cw = eventReceiver.isKeyDown(controls[CONTROL::CW_ROLL]);

        if (ccw && cw)
            ccw = cw = false;

        if (ccw)
                angularVelocity.setZ(angularVelocity.z() + ROLL_VELOCITY_ACCELERATION_STEP);
        else if (cw)
                angularVelocity.setZ(angularVelocity.z() - ROLL_VELOCITY_ACCELERATION_STEP);
        else {
            if (angularVelocity.z() > 0) {
                angularVelocity.setZ(angularVelocity.z() - ROLL_VELOCITY_DECELERATION_STEP);

                if (angularVelocity.z() < 0)
                    angularVelocity.setZ(0);
            }

            if (angularVelocity.z() < 0) {
                angularVelocity.setZ(angularVelocity.z() + ROLL_VELOCITY_DECELERATION_STEP);

                if (angularVelocity.z() > 0)
                    angularVelocity.setZ(0);
            }
        }

        if (angularVelocity.absolute().z() > MAX_ROLL_VELOCITY)
            angularVelocity.setZ(sign(angularVelocity.z()) * MAX_ROLL_VELOCITY);
    }

    angularVelocity = angularVelocity.rotate(axis, angle);
    notNanAssert(angularVelocity);
    plane.setAngularVelocity(angularVelocity);

    sideImpulse = sideImpulse.rotate(axis, angle);
    sideImpulse.setZ(0);
    notNanAssert(sideImpulse);
    plane.rigidBody().applyCentralImpulse(sideImpulse);

    plane.rigidBody().applyCentralImpulse(btVector3(0, 0, FORWARD_IMPULSE));

    // air resistance simulation
    btVector3 linearVelocity = -plane.getLinearVelocity();
    linearVelocity *= 0.00001f*linearVelocity.length();
    notNanAssert(linearVelocity);
    plane.rigidBody().applyCentralImpulse(linearVelocity);
}
