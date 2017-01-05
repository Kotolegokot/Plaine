/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLANECONTROL_H
#define PLANECONTROL_H

#include <algorithm>
#include "EventReceiver.hpp"
#include "Config.hpp"
#include "Plane.hpp"
#include "util/constants.hpp"

class PlaneControl
{
public:
    PlaneControl(Plane &plane, const Controls &controls = Controls());
    void handle(EventReceiver &eventReceiver);

private:
    static constexpr btScalar FORWARD_IMPULSE = 150;

    static constexpr btScalar MAX_PITCH_ANGLE = 0.3f;
    static constexpr btScalar MAX_YAW_ANGLE = 0.40f;

    static constexpr btScalar MAX_ROLL_VELOCITY = 2.0f;
    static constexpr btScalar ROLL_VELOCITY_ACCELERATION_STEP = 0.1f;
    static constexpr btScalar ROLL_VELOCITY_DECELERATION_STEP = 0.5f;

    static constexpr btScalar LEFT_IMPULSE = 50.0f;
    static constexpr btScalar RIGHT_IMPULSE = 50.0f;
    static constexpr btScalar UP_IMPULSE = 50.0f;
    static constexpr btScalar DOWN_IMPULSE = 50.0f;

    Plane &plane;
    const Controls &controls;
};

#endif // PLANECONTROL_H
