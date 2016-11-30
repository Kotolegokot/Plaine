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

#ifndef PLANE_H
#define PLANE_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "MotionState.h"
#include "Body.h"

using namespace irr;

class Plane : public Body {
public:
    Plane(btDynamicsWorld &physicsWorld, std::unique_ptr<btRigidBody> rigidBody) :
        Body(physicsWorld, std::move(rigidBody)) {}

    void explode() { m_exploded = true; }
    bool exploded() const { return m_exploded; }

    void addScore(long score)
    {
        if (score > 0)
            m_score += score * positiveMultiplier;
        else
            m_score += score * negativeMultiplier;

        if (m_score < 0)
            m_score = 0;
    }

    long score() const { return m_score; }
    void nullifyScore() { m_score = 0; }

    int positiveMultiplier = 1;
    int negativeMultiplier = 1;
private:
    bool m_exploded = false;
    long m_score = 0;
};

#endif // PLANE_H
