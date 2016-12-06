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

#include "ObstacleGenerator.h"

using namespace irr;

ObstacleGenerator::ObstacleGenerator(btDynamicsWorld &world, IrrlichtDevice &device, btScalar farValue, btScalar buffer) :
    world(world), device(device), m_farValue(farValue), m_buffer(buffer) {}

void ObstacleGenerator::generate(const btVector3 &playerPosition, const ChunkDB &chunkDB)
{
    unsigned long obstaclesGenerated = 0;

    const Cuboid<long> view = fieldOfView(playerPosition) / CELL_LENGTH; //field of view in cells
    Vector3<int> cell; // current cell to generate

    for (cell.z = back(view); cell.z <= front(generatedCuboid); cell.z++) {
        for (cell.x = left(view); cell.x < left(generatedCuboid); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstaclesGenerated += insertCell(cell, chunkDB);

        for (cell.x = left(generatedCuboid); cell.x <= right(generatedCuboid); cell.x++) {
            for (cell.y = bottom(view); cell.y < bottom(generatedCuboid); cell.y++)
                obstaclesGenerated += insertCell(cell, chunkDB);

            for (cell.y = top(generatedCuboid) + 1; cell.y <= top(view); cell.y++)
                obstaclesGenerated += insertCell(cell, chunkDB);
        }

        for (cell.x = right(generatedCuboid) + 1; cell.x <= right(view); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstaclesGenerated += insertCell(cell, chunkDB);
    }

    for (cell.z = front(generatedCuboid) + 1; cell.z <= front(view); cell.z++)
        for (cell.x = left(view); cell.x <= right(view); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstaclesGenerated += insertCell(cell, chunkDB);

    Log::debug(obstaclesGenerated, " obstacles generated");

    obstacleCount += obstaclesGenerated;
    generatedCuboid = view;

    removeLeftBehind(playerPosition.z());
}

Cuboid<btScalar> ObstacleGenerator::fieldOfView(const btVector3 &playerPosition) const
{
    return {
        // left bottom back point
        { playerPosition.x() - farValueWithBuffer(),
          playerPosition.y() - farValueWithBuffer(),
          playerPosition.z() },
        // right top front point
        { playerPosition.x() + farValueWithBuffer(),
          playerPosition.y() + farValueWithBuffer(),
          playerPosition.z() + farValueWithBuffer() }
    };
}

Vector3<int> ObstacleGenerator::cellToChunk(const Vector3<int> &cell)
{
    Vector3<int> result = cell;

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

Vector3<int> ObstacleGenerator::relativeCellPos(const Vector3<int> &cell, const Vector3<int> &chunk)
{
    return cell - chunk * CHUNK_SIZE;
}

std::size_t ObstacleGenerator::insertCell(Vector3<int> cell, const ChunkDB &chunkDB)
{
    Vector3<int> chunk = cellToChunk(cell);

    std::size_t chunkIndex = (chunk.x * chunk.y * chunk.z + chunk.x + chunk.y + chunk.z)
            % chunkDB.size();

    return chunkDB[chunkIndex].produceCell(world, device,
                                           chunk.toBulletVector3() * CHUNK_LENGTH,
                                           relativeCellPos(cell, chunk),
                                           m_obstacles);
}

// removes obstacles behind the player
void ObstacleGenerator::removeLeftBehind(btScalar playerZ)
{
    std::size_t count = 0;
    for (auto it = m_obstacles.begin();
        it != m_obstacles.end() && count < 100; count++)
    {
        if ((*it)->getPosition().z() < playerZ - m_buffer ||
            (*it)->getPosition().z() > playerZ + farValueWithBuffer() * 2)
        {
            it->reset();
            it = m_obstacles.erase(it);
            obstacleCount--;
        } else {
            it++;
        }
    }
}

std::size_t ObstacleGenerator::obstacles() const
{
    return obstacleCount;
}

btScalar ObstacleGenerator::farValueWithBuffer() const
{
    return m_farValue + m_buffer;
}

btScalar ObstacleGenerator::farValue() const
{
    return m_farValue;
}

btScalar ObstacleGenerator::buffer() const
{
    return m_buffer;
}
