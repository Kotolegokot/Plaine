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

#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "options.h"
#include "MotionState.h"
#include "IObstacle.h"
#include "IBody.h"
#include "Randomizer.h"
#include "Patterns.h"
#include "Chunk.h"

using namespace irr;

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;
using ChunkDB = std::array<Chunk<CHUNK_SIZE>, CHUNK_DB_SIZE>;

// this class is responsible for generating obstacles on the fly
class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue = 1500, btScalar buffer = CHUNK_LENGTH);
    void generate(const core::vector3df &playerPosition, const ChunkDB &chunkDB);
    u32 getCubeCount() const;

    void setFarValue(btScalar value);
    btScalar getFarValue() const;
    void setBuffer(btScalar buffer);
    btScalar getBuffer() const;

private:
    Cuboid<btScalar> fieldOfView(const core::vector3df &playerPosition) const;

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

    IrrlichtDevice &device;
    std::list<std::unique_ptr<IObstacle>> obstacles;

    u32 obstacleCount = 0;

    btScalar farValue = 0;
    // buffer is used to generate obstacles a bit farther than
    //      the camera's far value so that player sees them
    //      smoothly floating into the view range
    btScalar buffer = 0;

    // cuboid denoting generated part of the world in cells
    // but its rear face doesn't matter
    Cuboid<long> generatedCuboid;

    btDynamicsWorld &world; // physics world
};

#endif // OBSTACLEGENERATOR_H
