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
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

class Explosion
{
public:
    Explosion(btDynamicsWorld &world,
              const btVector3 &pos = btVector3(0, 0, 0), btScalar radius = 1000);
    ~Explosion();

    void set_position(const btVector3 &pos);
    btVector3 position() const;

    void explode();

private:
    btDynamicsWorld &world;
    btScalar radius = 0.0f;
    std::unique_ptr<btGhostObject> explosion_obj;
};
