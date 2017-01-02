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

#include "Explosion.hpp"

Explosion::Explosion(btDynamicsWorld &world, const btVector3 &pos, btScalar radius) :
    world(world), radius(radius)
{
    explosion_obj = std::make_unique<btGhostObject>();
    explosion_obj->setCollisionShape(new btSphereShape(radius));
    explosion_obj->setWorldTransform(btTransform(btQuaternion::getIdentity(), pos));
    explosion_obj->setCollisionFlags(explosion_obj->getCollisionFlags() |
                                     btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

    world.addCollisionObject(explosion_obj.get());
    world.getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);
}

Explosion::~Explosion()
{
    world.removeCollisionObject(explosion_obj.get());
}

void Explosion::set_position(const btVector3 &position)
{
    explosion_obj->setWorldTransform(btTransform(btQuaternion::getIdentity(), position));
}

btVector3 Explosion::position() const
{
    return explosion_obj->getWorldTransform().getOrigin();
}

void Explosion::explode()
{
    for (int i = 0; i < explosion_obj->getNumOverlappingObjects(); i++)
    {
        btRigidBody &body = *dynamic_cast<btRigidBody *>(explosion_obj->getOverlappingObject(i));
        if (body.getUserIndex() == 1) continue; // if plane, skip it

        btVector3 force = body.getCenterOfMassPosition() - this->position();
        btScalar distance = force.length();
        force.safeNormalize();
        force *= (radius - distance * 0.1f) * 100.0f;
        body.activate();
        body.applyForce(force, btVector3(0, 0, 0));
    }
}
