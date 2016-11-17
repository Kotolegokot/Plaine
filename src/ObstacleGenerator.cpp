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

#include "ObstacleGenerator.h"
#include "Chunk.h"
#include "util.h"
#include "options.h"

using namespace irr;

constexpr btScalar ObstacleGenerator::CELL_SIZE;
constexpr btScalar ObstacleGenerator::CHUNK_LENGTH;

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue, btScalar buffer) :
    device(device), farValue(farValue), buffer(buffer), world(world) {}

void ObstacleGenerator::generate(const core::vector3df &playerPosition,
                                 const ChunkDB &chunkDB)
{
    #if DEBUG_OUTPUT
        unsigned long obstaclesGenerated = 0;
    #endif
    long edgeLeft, edgeRight, edgeBottom, edgeTop, edgeBack, edgeFront;
    stickToGrid(playerPosition, edgeLeft, edgeRight, edgeBottom, edgeTop, edgeBack, edgeFront);

    for (long z = edgeBack; z <= generatedEdgeFront; z++) {
        for (long x = edgeLeft; x < generatedEdgeLeft; x++)
            for (long y = edgeBottom; y <= edgeTop; y++)
                obstaclesGenerated += generateChunk(x, y, z, chunkDB);

        for (long x = generatedEdgeLeft; x <= generatedEdgeRight; x++) {
            for (long y = edgeBottom; y < generatedEdgeBottom; y++)
                obstaclesGenerated += generateChunk(x, y, z, chunkDB);

            for (long y = generatedEdgeTop + 1; y <= edgeTop; y++)
                obstaclesGenerated += generateChunk(x, y, z, chunkDB);
        }

        for (long x = generatedEdgeRight + 1; x <= edgeRight; x++)
            for (long y = edgeBottom; y <= edgeTop; y++)
                obstaclesGenerated += generateChunk(x, y, z, chunkDB);
    }

    for (long z = generatedEdgeFront + 1; z <= edgeFront; z++)
        for (long x = edgeLeft; x <= edgeRight; x++)
            for (long y = edgeBottom; y <= edgeTop; y++)
                obstaclesGenerated += generateChunk(x, y, z, chunkDB);

    #if DEBUG_OUTPUT
        std::cout << obstaclesGenerated << " obstacles generated" << std::endl;
    #endif // DEBUG_OUTPUT

    obstacleCount += obstaclesGenerated;

    generatedEdgeLeft = edgeLeft;
    generatedEdgeRight = edgeRight;
    generatedEdgeTop = edgeTop;
    generatedEdgeBottom = edgeBottom;
    generatedEdgeFront = edgeFront;

    removeLeftBehind(playerPosition.Z);
}

void ObstacleGenerator::stickToGrid(const core::vector3df &playerPosition, long &edgeLeft, long &edgeRight,
                                    long &edgeBottom, long &edgeTop, long &edgeBack, long &edgeFront) const
{
    edgeLeft = (playerPosition.X - farValueWithBuffer()) / CHUNK_LENGTH;
    edgeRight = (playerPosition.X + farValueWithBuffer()) / CHUNK_LENGTH;
    edgeBottom = (playerPosition.Y - farValueWithBuffer()) / CHUNK_LENGTH;
    edgeTop = (playerPosition.Y + farValueWithBuffer()) / CHUNK_LENGTH;
    edgeBack = playerPosition.Z / CHUNK_LENGTH;
    edgeFront = (playerPosition.Z + farValueWithBuffer()) / CHUNK_LENGTH;
}

unsigned long ObstacleGenerator::generateChunk(long x, long y, long z, const ChunkDB &chunkDB)
{
    std::size_t chunkIndex = Randomizer::getInt(0, chunkDB.size() - 1);

    return chunkDB[chunkIndex].produce(world, device, CELL_SIZE,
                                       btVector3(x, y, z) * CHUNK_LENGTH, obstacles);
}

// removes obstacles behind the player
void ObstacleGenerator::removeLeftBehind(btScalar playerZ)
{
    size_t count = 0;
    for (auto it = obstacles.begin();
        it != obstacles.end() && count < 100; count++)
    {
        if ((*it)->getPosition().z() < playerZ - buffer ||
            (*it)->getPosition().z() > playerZ + farValueWithBuffer() * 2)
        {
            it->reset();
            it = obstacles.erase(it);
            obstacleCount--;
        } else {
            it++;
        }
    }
}

u32 ObstacleGenerator::getCubeCount() const
{
    return obstacleCount;
}

btScalar ObstacleGenerator::farValueWithBuffer() const
{
    return farValue + buffer;
}

void ObstacleGenerator::setFarValue(btScalar value)
{
    farValue = value;
}

btScalar ObstacleGenerator::getFarValue() const
{
    return farValue;
}

void ObstacleGenerator::setBuffer(btScalar buffer)
{
    this->buffer = buffer;
}

btScalar ObstacleGenerator::getBuffer() const
{
    return buffer;
}
