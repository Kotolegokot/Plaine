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

#ifndef PLANECONTROL_H
#define PLANECONTROL_H

#include <algorithm>
#include "EventReceiver.h"
#include "Config.h"
#include "Plane.h"
#include "util.h"

class PlaneControl
{
public:
    PlaneControl(Plane &plane, const Controls &controls = Controls());
    void handle(EventReceiver &eventReceiver);

private:
    static constexpr btScalar MAX_PITCH_ANGLE = 0.3f;
    static constexpr btScalar MAX_PITCH_VELOCITY = 1.0f;

    static constexpr btScalar MAX_YAW_ANGLE = 0.30f;
    static constexpr btScalar MAX_YAW_VELOCITY = 1.0f;

    static constexpr btScalar MAX_ROLL_VELOCITY = 3.0f;
    static constexpr btScalar ROLL_VELOCITY_STEP = 0.2f;

    static constexpr btScalar LEFT_IMPULSE = 50.0f;
    static constexpr btScalar RIGHT_IMPULSE = 50.0f;
    static constexpr btScalar UP_IMPULSE = 50.0f;
    static constexpr btScalar DOWN_IMPULSE = 50.0f;

    Plane &plane;
    const Controls &controls;
};

#endif // PLANECONTROL_H
