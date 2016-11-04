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

PlaneControl::PlaneControl(Plane &plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver &eventReceiver)
{
    btVector3 axis;
    btScalar angle;
    plane.getAxisAngleRotation(axis, angle);

    btVector3 rotation = plane.getEulerRotation();

    btVector3 angularVelocity = plane.getAngularVelocity();

    btVector3 sideImpulse(0, 0, 0);

    // turn up and down
    {
        bool up = eventReceiver.isKeyDown(controls[CONTROL::UP]);
        bool down = eventReceiver.isKeyDown(controls[CONTROL::DOWN]);

        if (!(up && down)) {
            if (up) {
                sideImpulse += btVector3(0, upImpulse, 0);

                if (angularVelocity.x() - 0.3f >= -maxPitchVelocity)
                    angularVelocity.setX(angularVelocity.x() - 0.3f);
            }

            if (down) {
                sideImpulse += btVector3(0, -downImpulse, 0);

                if (angularVelocity.x() + 0.3f <= maxPitchVelocity)
                    angularVelocity.setX(angularVelocity.x() + 0.3f);
            }
        }

        if (rotation.x() < -maxPitchAngle)
            angularVelocity.setX(0);
        else if (rotation.x() > maxPitchAngle)
            angularVelocity.setX(0);

        if (!up && !down) {
            if (rotation.x() < 0.0f)
                angularVelocity.setX(rotation.absolute().x() * 2.0f);

            if (rotation.x() > 0.0f)
                angularVelocity.setX(rotation.absolute().x() * -2.0f);
        }
    }

    // turn left and right
    {
        bool left = eventReceiver.isKeyDown(controls[CONTROL::LEFT]);
        bool right = eventReceiver.isKeyDown(controls[CONTROL::RIGHT]);

        if (!(left && right)) {
            if (left) {
                sideImpulse += btVector3(-leftImpulse, 0, 0);

                if (angularVelocity.y() - 0.3f >= -maxYawVelocity)
                    angularVelocity.setY(angularVelocity.y() - 0.3f);
            }

            if (right) {
                sideImpulse += btVector3(rightImpulse, 0, 0);

                if (angularVelocity.y() + 0.3f <= maxYawVelocity)
                    angularVelocity.setY(angularVelocity.y() + 0.3f);
            }
        }

        if (rotation.y() < -maxYawAngle)
            angularVelocity.setY(0);
        if (rotation.y() > maxYawAngle)
            angularVelocity.setY(0);

        if (!left && !right) {
            if (rotation.y() < 0.0f) {
                angularVelocity.setY(rotation.absolute().y() * 2.0f);
            }

            if (rotation.y() > 0.0f) {
                angularVelocity.setY(rotation.absolute().y() * -2.0f);
            }
        }
    }

    // rool ccw and cw
    {
        bool ccw = eventReceiver.isKeyDown(controls[CONTROL::CCW_ROLL]);
        bool cw = eventReceiver.isKeyDown(controls[CONTROL::CW_ROLL]);

        if (!(ccw && cw)) {
            if (ccw)
                if (angularVelocity.z() + rollVelocityStep <= maxRollVelocity)
                    angularVelocity.setZ(angularVelocity.z() + rollVelocityStep);

            if (cw)
                if (angularVelocity.z() - rollVelocityStep >= -maxRollVelocity)
                    angularVelocity.setZ(angularVelocity.z() - rollVelocityStep);
        }

        if (!ccw && !cw) {
            if (angularVelocity.z() > 0) {
                angularVelocity.setZ(angularVelocity.z() - rollVelocityStep);

                if (angularVelocity.z() < 0)
                    angularVelocity.setZ(0);
            }

            if (angularVelocity.z() < 0) {
                angularVelocity.setZ(angularVelocity.z() + rollVelocityStep);

                if (angularVelocity.z() > 0)
                    angularVelocity.setZ(0);
            }
        }
    }

    angularVelocity = angularVelocity.rotate(axis, angle);
    plane.setAngularVelocity(angularVelocity);

    sideImpulse = sideImpulse.rotate(axis, angle);
    sideImpulse.setZ(0);
    plane.getRigidBody().applyCentralImpulse(sideImpulse);

    plane.getRigidBody().applyCentralImpulse(btVector3(0, 0, 50));

    // air resistance simulation
    btVector3 linearVelocity = -plane.getRigidBody().getLinearVelocity();
    linearVelocity *= 0.00001f*linearVelocity.length();
    plane.getRigidBody().applyCentralImpulse(linearVelocity);
}
