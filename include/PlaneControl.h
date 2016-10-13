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
    Plane &plane;
    const Controls &controls;
};

#endif // PLANECONTROL_H
