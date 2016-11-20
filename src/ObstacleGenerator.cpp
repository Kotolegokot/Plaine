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
#include "util.h"
#include "options.h"

using namespace irr;

constexpr btScalar ObstacleGenerator::STEP;
constexpr btScalar ObstacleGenerator::START;

ObstacleGenerator::ObstacleGenerator(btDynamicsWorld &world, IrrlichtDevice &device, btScalar farValue, btScalar buffer) :
    world(world), device(device), m_farValue(farValue), m_buffer(buffer) {}

ObstacleGenerator::~ObstacleGenerator()
{
    // remove all stored cubes
    while (!m_obstacles.empty())
        m_obstacles.pop_front();
}

void ObstacleGenerator::generate(const btVector3 &playerPosition)
{
    // number of obstacles generated within this tick
    #if DEBUG_OUTPUT
    unsigned long obstacleGenerated = 0;
    auto handleCell = [&obstacleGenerated, this](btScalar x, btScalar y, btScalar z)
    #else
    auto handleCell =  [this](btScalar x, btScalar y, btScalar z)
    #endif // DEBUG_OUTPUT
    {
        if (z < START)
            return;

        btScalar newX = x + Randomizer::getFloat(-100, 100);
        btScalar newY = y + Randomizer::getFloat(-100, 100);
        btScalar newZ = z + Randomizer::getFloat(-100, 100);

        // create obstacles and add them to the deque
        switch (Randomizer::getInt(0, 7)) {
        case 0:
            {
                Tunnel tunnel(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(100, 200), Randomizer::getFloat(300, 600));
                tunnel.addObstaclesToList(m_obstacles);
                obstacleCount += tunnel.getObstacleCount();
                #if DEBUG_OUTPUT
                    obstacleGenerated += tunnel.getObstacleCount();
                #endif // DEBUG_OUTPUT
                break;
            }
        case 1:
            {
                Crystal crystal(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(50.f, 100.f), Randomizer::getFloat(300.f, 600.f));
                crystal.addObstaclesToList(m_obstacles);
                obstacleCount += crystal.getObstacleCount();
                #if DEBUG_OUTPUT
                    obstacleGenerated += crystal.getObstacleCount();
                #endif // DEBUG_OUTPUT
                break;
            }
        case 2:
            {
                break;
                Valley<5> valley(world, device, btVector3(newX, newY, newZ), 600);
                valley.addObstaclesToList(m_obstacles);
                obstacleCount += valley.getObstacleCount();
                #if DEBUG_OUTPUT
                    obstacleGenerated += valley.getObstacleCount();
                #endif
                break;
            }
        case 3:
            {
                std::unique_ptr<IObstacle> obstacle =
                    std::make_unique<Icosahedron>(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(150.f, 350.f));
                m_obstacles.push_back(std::move(obstacle));
                obstacleCount++;
                #if DEBUG_OUTPUT
                    obstacleGenerated++;
                #endif // DEBUG_OUTPUT
                break;
            }
        case 4:
            {
                std::unique_ptr<IObstacle> obstacle =
                    std::make_unique<Icosphere2>(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(50.f, 200.f));
                m_obstacles.push_back(std::move(obstacle));
                obstacleCount++;
                #if DEBUG_OUTPUT
                    obstacleGenerated++;
                #endif // DEBUG_OUTPUT
                break;
            }
        case 5:
            {
                std::unique_ptr<IObstacle> obstacle =
                    std::make_unique<Tetrahedron>(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(200.f, 400.f));
                m_obstacles.push_back(std::move(obstacle));
                obstacleCount++;
                #if DEBUG_OUTPUT
                    obstacleGenerated++;
                #endif // DEBUG_OUTPUT
                break;
            }
        case 6:
            {
                //Emptiness empt(nothing);
                //empt.add_to_the_world(world, device);
                break;
            }
        default:
            break;
        }

        //if (Randomizer::getInt(1, 1) == 1)
        //    body->getRigidBody()->applyTorqueImpulse(btVector3(Randomizer::getFloat(-10000, 10000), Randomizer::getFloat(-10000, 10000), Randomizer::getFloat(-10000, 10000))*body->getMass());
        //if (Randomizer::getInt(1, 1) == 1)
        //    body->getRigidBody()->applyCentralImpulse(btVector3(Randomizer::getFloat(-100, 100), Randomizer::getFloat(-100, 100), Randomizer::getFloat(-100, 100))*body->getMass());
    };

    // The Z loop must be the first here, because the obstacle deque must be sorted by obstacle' Z coordinate
    for (btScalar z = preciseEdge(playerPosition.z() - m_buffer); z <= generatedEdgeZ; z += STEP) {

        for (btScalar x = preciseEdge(playerPosition.x() - farValueWithBuffer()); x < generatedEdgeLeft; x += STEP)
            for (btScalar y = preciseEdge(playerPosition.y() - farValueWithBuffer());
                y <= preciseEdge(playerPosition.y() + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }

        for (btScalar x = generatedEdgeLeft; x <= generatedEdgeRight; x += STEP) {
            for (btScalar y = preciseEdge(playerPosition.y() - farValueWithBuffer()); y < generatedEdgeBottom; y += STEP)
                handleCell(x, y, z);

            for (btScalar y = generatedEdgeTop + STEP; y <= preciseEdge(playerPosition.y() + farValueWithBuffer()); y += STEP)
                handleCell(x, y, z);
        }

        for (btScalar x = generatedEdgeRight + STEP; x <= preciseEdge(playerPosition.x() + farValueWithBuffer()); x += STEP)
            for (btScalar y = preciseEdge(playerPosition.y() - farValueWithBuffer());
                y <= preciseEdge(playerPosition.y() + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }
    }

    for (btScalar z = generatedEdgeZ + STEP; z <= preciseEdge(playerPosition.z() + farValueWithBuffer()); z += STEP)
        for (btScalar x = preciseEdge(playerPosition.x() - farValueWithBuffer());
            x <= preciseEdge(playerPosition.x() + farValueWithBuffer()); x += STEP)
            for (btScalar y = preciseEdge(playerPosition.y() - farValueWithBuffer());
                y <= preciseEdge(playerPosition.y() + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }

    #if DEBUG_OUTPUT
        std::cout << obstacleGenerated << " obstacles generated" << std::endl;
    #endif // DEBUG_OUTPUT

    // these are the edges of the generated zone
    // the part of the level behind them is generated and must not be touched
    generatedEdgeLeft = preciseEdge(playerPosition.x() - farValueWithBuffer());
    generatedEdgeRight = preciseEdge(playerPosition.x() + farValueWithBuffer());
    generatedEdgeBottom = preciseEdge(playerPosition.y() - farValueWithBuffer());
    generatedEdgeTop = preciseEdge(playerPosition.y() + farValueWithBuffer());
    generatedEdgeZ = preciseEdge(playerPosition.z() + farValueWithBuffer());

    // remove obstacles behind the player to save some memory
    removeLeftBehind(playerPosition.z());
}

btScalar ObstacleGenerator::preciseEdge(btScalar edge) const
{
    return floor(edge / STEP)*STEP;
}

// removes obstacles behind the player
void ObstacleGenerator::removeLeftBehind(btScalar playerZ)
{
    size_t count = 0;
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
