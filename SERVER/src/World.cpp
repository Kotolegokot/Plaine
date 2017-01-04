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

#include "World.hpp"

void check_collisions(btDynamicsWorld *physicsWorld, btScalar timeStep);

World::World(const ChunkDB &chunkDB) :
    m_broadphase(std::make_unique<btDbvtBroadphase>()),
    m_collisionConfiguration(std::make_unique<btDefaultCollisionConfiguration>()),
    m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get())),
    m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    m_chunkDB(chunkDB)
{
    btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher.get());

    m_physicsWorld = std::make_unique<btDiscreteDynamicsWorld>
            (m_dispatcher.get(), m_broadphase.get(),
             m_solver.get(), m_collisionConfiguration.get());
    m_physicsWorld->setInternalTickCallback(&check_collisions, static_cast<void *>(this), true);
    m_physicsWorld->setGravity({ 0, 0, 0 });

    m_plane = std::make_unique<Plane>(*m_physicsWorld, PlaneFactory().produce({ 0, 0, 0 }));
    m_explosion = std::make_unique<Explosion>(*m_physicsWorld,
                                              m_plane->rigid_body().getCenterOfMassPosition(),
                                              1000);

    m_generator = std::make_unique<Generator>(*m_physicsWorld, 5000, 300);
}

void World::stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
    m_explosion->set_position(m_plane->rigid_body().getCenterOfMassPosition());
    m_gameOver = m_plane->exploded();

    m_physicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
}

void World::generate()
{
    m_generator->generate(m_plane->rigid_body().getCenterOfMassPosition(), m_chunkDB);
}

bool World::game_over() const
{
    return m_gameOver;
}

std::size_t World::obstacles() const
{
    return m_generator->obstacles();
}

Plane &World::plane()
{
    return *m_plane;
}

void check_collisions(btDynamicsWorld *physicsWorld, btScalar /* timeStep */)
{
    World &world = *static_cast<World *>(physicsWorld->getWorldUserInfo());

    int numManifolds = physicsWorld->getDispatcher()->getNumManifolds();

    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold *contactManifold = physicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        auto objA = static_cast<const btCollisionObject *>(contactManifold->getBody0());
        auto objB = static_cast<const btCollisionObject *>(contactManifold->getBody1());

        // plane must be always objA
        if (objB == &world.plane().rigid_body())
            std::swap(objA, objB);

        bool plane = objA == &world.plane().rigid_body();

        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++) {
            btManifoldPoint &pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() <= 0.0f && pt.getAppliedImpulse() > 0) {
                if (plane) {
                    if (pt.getAppliedImpulse() > EXPLOSION_THRESHOLD) {
                        world.plane().explode();
                    } else if (!world.plane().exploded()) {
                        world.plane().add_score(-pt.getAppliedImpulse());
                    }
                }
            }
        }
    }
}
