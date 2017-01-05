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

#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "MotionState.h"
#include "Patterns.h"
#include "Chunk.h"
#include "Log.h"
#include "util/Randomizer.h"
#include "util/Cuboid.h"
#include "util/other.h"
#include "util/options.h"

using namespace irr;

using ChunkDB = std::array<Chunk<CHUNK_SIZE>, CHUNK_DB_SIZE>;

// this class is responsible for generating obstacles on the fly
class ObstacleGenerator
{
public:
    ObstacleGenerator(btDynamicsWorld &world, IrrlichtDevice &device, btScalar farValue = 1500,
                      btScalar buffer = CHUNK_LENGTH);

    void generate(const btVector3 &playerPosition, const ChunkDB &chunkDB);

    std::size_t obstacles() const;
    btScalar farValue() const;
    btScalar buffer() const;

private:
    Cuboid<btScalar> fieldOfView(const btVector3 &playerPosition) const;

    // takes chunk coordinates and generate its appropriate part
    std::size_t insertCell(Vector3<int> cell, const ChunkDB &chunkDB);

    static Vector3<int> cellToChunk(const Vector3<int> &cell);
    static Vector3<int> relativeCellPos(const Vector3<int> &cell, const Vector3<int> &chunk);

    static long back(const Cuboid<long> cuboid) { return cuboid.p1.z; }
    static long front(const Cuboid<long> cuboid) { return cuboid.p2.z; }
    static long left(const Cuboid<long> cuboid) { return cuboid.p1.x; }
    static long right(const Cuboid<long> cuboid) { return cuboid.p2.x; }
    static long bottom(const Cuboid<long> cuboid) { return cuboid.p1.y; }
    static long top(const Cuboid<long> cuboid) { return cuboid.p2.y; }

    void removeLeftBehind(btScalar playerZ);
    btScalar farValueWithBuffer() const;

    btDynamicsWorld &world;
    IrrlichtDevice &device;
    std::list<std::unique_ptr<Body>> m_obstacles;

    u32 obstacleCount = 0;

    btScalar m_farValue = 0;
    // buffer is used to generate obstacles a bit farther than
    //      the camera's far value so that player sees them
    //      smoothly floating into the view range
    btScalar m_buffer = 0;

    // cuboid denoting generated part of the world in cells
    // but its rear face doesn't matter
    Cuboid<long> generatedCuboid;
};

#endif // OBSTACLEGENERATOR_H
