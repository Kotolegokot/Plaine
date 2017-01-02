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

#include "Generator.hpp"

Generator::Generator(btDynamicsWorld &world, btScalar far_value, btScalar buffer) :
    world(world), m_farValue(far_value), m_buffer(buffer) {}

Generator::~Generator()
{
    while (!m_obstacles.empty()) {
        world.removeRigidBody(m_obstacles.front().get());
        m_obstacles.pop_front();
    }
}

void Generator::generate(const btVector3 &player_pos, const ChunkDB &chunk_db)
{
    unsigned long obstacles_generated = 0;

    const Cuboid<long> view = filed_of_view(player_pos) / CELL_LENGTH; //field of view in cells
    Vector3<int> cell; // current cell to generate

    for (cell.z = back(view); cell.z <= front(generated_cuboid); cell.z++) {
        for (cell.x = left(view); cell.x < left(generated_cuboid); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstacles_generated += insert_cell(cell, chunk_db);

        for (cell.x = left(generated_cuboid); cell.x <= right(generated_cuboid); cell.x++) {
            for (cell.y = bottom(view); cell.y < bottom(generated_cuboid); cell.y++)
                obstacles_generated += insert_cell(cell, chunk_db);

            for (cell.y = top(generated_cuboid) + 1; cell.y <= top(view); cell.y++)
                obstacles_generated += insert_cell(cell, chunk_db);
        }

        for (cell.x = right(generated_cuboid) + 1; cell.x <= right(view); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstacles_generated += insert_cell(cell, chunk_db);
    }

    for (cell.z = front(generated_cuboid) + 1; cell.z <= front(view); cell.z++)
        for (cell.x = left(view); cell.x <= right(view); cell.x++)
            for (cell.y = bottom(view); cell.y <= top(view); cell.y++)
                obstacles_generated += insert_cell(cell, chunk_db);

    obstacleCount += obstacles_generated;
    generated_cuboid = view;

    remove_left_behind(player_pos.z());
}

Cuboid<btScalar> Generator::filed_of_view(const btVector3 &playerPosition) const
{
    return {
        // left bottom back point
        { playerPosition.x() - far_value_with_buf(),
          playerPosition.y() - far_value_with_buf(),
          playerPosition.z() },
        // right top front point
        { playerPosition.x() + far_value_with_buf(),
          playerPosition.y() + far_value_with_buf(),
          playerPosition.z() + far_value_with_buf() }
    };
}

Vector3<int> Generator::cell_to_chunk(const Vector3<int> &cell)
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

Vector3<int> Generator::rel_cell_pos(const Vector3<int> &cell, const Vector3<int> &chunk)
{
    return cell - chunk * CHUNK_SIZE;
}

std::size_t Generator::insert_cell(Vector3<int> cell, const ChunkDB &chunkDB)
{
    Vector3<int> chunk = cell_to_chunk(cell);

    std::size_t chunkIndex = (chunk.x * chunk.y * chunk.z + chunk.x + chunk.y + chunk.z)
            % chunkDB.size();

    return chunkDB[chunkIndex].produce_cell(world,
                                           chunk * CHUNK_LENGTH,
                                           rel_cell_pos(cell, chunk),
                                           m_obstacles);
}

// removes obstacles behind the player
void Generator::remove_left_behind(btScalar playerZ)
{
    std::size_t count = 0;
    for (auto it = m_obstacles.begin();
        it != m_obstacles.end() && count < 100; count++)
    {
        if ((*it)->getCenterOfMassPosition().z() < playerZ - m_buffer ||
            (*it)->getCenterOfMassPosition().z() > playerZ + far_value_with_buf() * 2)
        {
            world.removeRigidBody((*it).get());
            it->reset(); // TODO: remove this line
            it = m_obstacles.erase(it);
            obstacleCount--;
        } else {
            it++;
        }
    }
}

std::size_t Generator::obstacles() const
{
    return obstacleCount;
}

btScalar Generator::far_value_with_buf() const
{
    return m_farValue + m_buffer;
}

btScalar Generator::far_value() const
{
    return m_farValue;
}

btScalar Generator::buffer() const
{
    return m_buffer;
}
