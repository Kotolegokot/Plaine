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

#include "DebugDrawer.h"

using namespace irr;

DebugDrawer::DebugDrawer(IrrlichtDevice &device)
    : device(device) {}

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &/*color*/)
{
    device.getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    device.getVideoDriver()->draw3DLine(
        core::vector3df(from.x(), from.y(), from.z()),
        core::vector3df(to.x(), to.y(), to.z()),
        video::SColor(255, 255, 255, 0));
}

void DebugDrawer::drawContactPoint(const btVector3 &/*PointOnB*/, const btVector3 &/*normalOnB*/,
                                   btScalar /*distance*/, int /*lifeTime*/,
                                   const btVector3 &/*color*/)
{

}

void DebugDrawer::reportErrorWarning(const char *warningString)
{
    std::cerr << "Error: " << warningString << std::endl;
}

void DebugDrawer::draw3dText(const btVector3 &/*location*/, const char */*textString*/)
{

}

void DebugDrawer::setDebugMode(int debugMode)
{
    this->debugMode = (DebugDrawModes) debugMode;
}

int DebugDrawer::getDebugMode() const
{
    return debugMode;
}
