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

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

class Plane {
    btDynamicsWorld &m_world;
    std::unique_ptr<btRigidBody> m_body;
    bool m_exploded = false;
    long m_score = 0;

public:
    explicit Plane(btDynamicsWorld &world, std::unique_ptr<btRigidBody> body) :
        m_world(world), m_body(std::move(body)) {}

    ~Plane()
    {
        m_world.removeRigidBody(m_body.get());
    }

    btRigidBody &rigid_body() { return *m_body; }
    const btRigidBody &rigid_body() const { return *m_body; }

    void explode() { m_exploded = true; }
    bool exploded() const { return m_exploded; }

    void add_score(long score)
    {
        if (score > 0)
            m_score += score * positive_mult;
        else
            m_score += score * negative_mult;

        if (m_score < 0)
            m_score = 0;
    }

    long score() const { return m_score; }
    void nullify_score() { m_score = 0; }

    int positive_mult = 1;
    int negative_mult = 1;
};
