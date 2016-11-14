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

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <algorithm>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

using namespace irr;

class Explosion
{
public:
    Explosion(btDynamicsWorld &world, IrrlichtDevice &device,
              const btVector3 &position = btVector3(0, 0, 0), btScalar radius = 1000);
    ~Explosion();

    void setPosition(const btVector3 &position);
    btVector3 getPosition() const;

    void explode();

private:
    btDynamicsWorld &world;
    IrrlichtDevice &device;
    btScalar radius = 0.0f;
    std::unique_ptr<btGhostObject> explosionObject;
    std::unique_ptr<scene::IParticleSystemSceneNode> particleSystem;


    void startAnimation();
};

#endif // EXPLOSION_H
