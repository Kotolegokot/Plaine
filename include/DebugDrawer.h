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

#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H

#include <iostream>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "Plane.h"

using namespace irr;

class DebugDrawer : public btIDebugDraw
{
public:
    DebugDrawer(IrrlichtDevice &deivce);

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
        int lifeTime,const btVector3 &color) override;
    virtual void reportErrorWarning(const char *warningString) override;
    virtual void draw3dText(const btVector3 &location, const char *textString) override;
    virtual void setDebugMode(int debugMode) override;
    virtual int getDebugMode() const override;

private:
    IrrlichtDevice &device;
    DebugDrawModes debugMode = DBG_NoDebug;
};

#endif // DEBUGDRAWER_H
