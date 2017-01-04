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

#include <iostream>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "Generator.hpp"
#include "Plane.hpp"
#include "bodies/PlaneFactory.hpp"
#include "Explosion.hpp"
#include "Chunk.hpp"

constexpr btScalar CAMERA_DISTANCE = 200;

constexpr btScalar EXPLOSION_THRESHOLD = 400.0f;

class World {
    friend struct ContactSensorCallback;
public:
    World(const ChunkDB &chunkDB);

    void stepSimulation(btScalar timeStep, int maxSubSteps = 1,
                        btScalar fixedTimeStep = btScalar(1.) / btScalar(60.));

    void generate();

    bool game_over() const;
    std::size_t obstacles() const;

    Plane &plane();
private:
    std::unique_ptr<btBroadphaseInterface> m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_physicsWorld;

    std::unique_ptr<Generator> m_generator;
    std::unique_ptr<Plane> m_plane;
    std::unique_ptr<Explosion> m_explosion;

    const ChunkDB &m_chunkDB;

    bool m_gameOver = false;
};
