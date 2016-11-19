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

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue, btScalar buffer) :
    device(device), farValue(farValue), buffer(buffer), world(world) {}

void ObstacleGenerator::generate(const core::vector3df &playerPosition,
                                 const ChunkDB &chunkDB)
{
    #if DEBUG_OUTPUT
        unsigned long obstaclesGenerated = 0;
    #endif

//    const Edges<long> chunkEdges = fieldOfView(playerPosition) / CHUNK_LENGTH;
    const Edges<long> cellEdges = fieldOfView(playerPosition) / CELL_LENGTH;

    for (long z = cellEdges.back; z <= generatedEdges.front; z++) {
        for (long x = cellEdges.left; x < generatedEdges.left; x++)
            for (long y = cellEdges.bottom; y <= cellEdges.top; y++)
                obstaclesGenerated += insertCell(x, y, z, chunkDB, cellEdges);

        for (long x = generatedEdges.left; x <= generatedEdges.right; x++) {
            for (long y = cellEdges.bottom; y < generatedEdges.bottom; y++)
                obstaclesGenerated += insertCell(x, y, z, chunkDB, cellEdges);

            for (long y = generatedEdges.top + 1; y <= cellEdges.top; y++)
                obstaclesGenerated += insertCell(x, y, z, chunkDB, cellEdges);
        }

        for (long x = generatedEdges.right + 1; x <= cellEdges.right; x++)
            for (long y = cellEdges.bottom; y <= cellEdges.top; y++)
                obstaclesGenerated += insertCell(x, y, z, chunkDB, cellEdges);
    }

    for (long z = generatedEdges.front + 1; z <= cellEdges.front; z++)
        for (long x = cellEdges.left; x <= cellEdges.right; x++)
            for (long y = cellEdges.bottom; y <= cellEdges.top; y++)
                obstaclesGenerated += insertCell(x, y, z, chunkDB, cellEdges);

    #if DEBUG_OUTPUT
        std::cout << obstaclesGenerated << " obstacles generated" << std::endl;
    #endif // DEBUG_OUTPUT

    obstacleCount += obstaclesGenerated;
    generatedEdges = cellEdges;

    removeLeftBehind(playerPosition.Z);
}

ObstacleGenerator::Edges<btScalar>
    ObstacleGenerator::fieldOfView(const core::vector3df &playerPosition) const
{
    return { playerPosition.X - farValueWithBuffer(),
             playerPosition.X + farValueWithBuffer(),
             playerPosition.Y - farValueWithBuffer(),
             playerPosition.Y + farValueWithBuffer(),
             playerPosition.Z,
             playerPosition.Z + farValueWithBuffer() };
}

Point3<int> ObstacleGenerator::cellToChunk(const Point3<int> &cell)
{
    Point3<int> result = cell;

    if (cell.x < 0)
        result.x++;
    if (cell.y < 0)
        result.y++;
    if (cell.z < 0)
        result.z++;

    result /= (long) CHUNK_SIZE;

    if (cell.x < 0)
        result.x--;
    if (cell.y < 0)
        result.y--;
    if (cell.z < 0)
        result.z--;

    return result;
}

Point3<int> ObstacleGenerator::relativeCellPos(const Point3<int> &cell, const Point3<int> &chunk)
{
    return cell - chunk * CHUNK_SIZE;
}

std::size_t ObstacleGenerator::insertCell(long x, long y, long z, const ChunkDB &chunkDB,
                                           Edges<long> cellEdges)
{
    Point3<int> cell(x, y, z);
    Point3<int> chunk = cellToChunk(cell);

    std::size_t chunkIndex = (chunk.x * chunk.y * chunk.z + chunk.x + chunk.y + chunk.z)
            % chunkDB.size();

    return chunkDB[chunkIndex].produceCell(world, device, CELL_LENGTH,
                                           btVector3(chunk.x, chunk.y, chunk.z) * CHUNK_LENGTH,
                                           obstacles, relativeCellPos(cell, chunk));
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
