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

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue, btScalar buffer) :
    device(device), farValue(farValue), buffer(buffer), world(world) {}

ObstacleGenerator::~ObstacleGenerator()
{
    // remove all stored cubes
    while (!obstacles.empty())
        obstacles.pop_front();
}

void ObstacleGenerator::generate(const core::vector3df &playerPosition)
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
        switch (Randomizer::getInt(0, 6)) {
        case 0:
            {
                Tunnel tunnel(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(100, 200), Randomizer::getFloat(300, 600));
                tunnel.addObstaclesToList(obstacles);
                obstacleCount += tunnel.getObstacleCount();
                #if DEBUG_OUTPUT
                    obstacleGenerated += tunnel.getObstacleCount();
                #endif // DEBUG_OUTPUT
                break;
            }
        case 1:
            {
                Crystal crystal(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(50.f, 100.f), Randomizer::getFloat(300.f, 600.f));
                crystal.addObstaclesToList(obstacles);
                obstacleCount += crystal.getObstacleCount();
                #if DEBUG_OUTPUT
                    obstacleGenerated += crystal.getObstacleCount();
                #endif // DEBUG_OUTPUT
                break;
            }
        case 2:
            {
                /*
                std::unique_ptr<IObstacle> obstacle =
                    std::make_unique<Box>(world, device, btVector3(newX, newY, newZ),
                        btVector3(Randomizer::getFloat(50.0f, 250.0f), Randomizer::getFloat(50.f, 250.f), Randomizer::getFloat(50.f, 250.f)));
                obstacles.push_back(std::move(obstacle));
                obstacleCount++;
                #if DEBUG_OUTPUT
                    obstacleGenerated++;
                #endif // DEBUG_OUTPUT*/
                break;
            }
        case 3:
            {
                std::unique_ptr<IObstacle> obstacle =
                    std::make_unique<Icosahedron>(world, device, btVector3(newX, newY, newZ), Randomizer::getFloat(150.f, 350.f));
                obstacles.push_back(std::move(obstacle));
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
                obstacles.push_back(std::move(obstacle));
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
                obstacles.push_back(std::move(obstacle));
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

        //if (int(Randomizer::getFloat(1, 1)) == 1)
        //    body->getRigidBody()->applyTorqueImpulse(btVector3(Randomizer::getFloat(-10000, 10000), Randomizer::getFloat(-10000, 10000), Randomizer::getFloat(-10000, 10000))*body->getMass());
        //if (int(Randomizer::getFloat(1, 1)) == 1)
        //    body->getRigidBody()->applyCentralImpulse(btVector3(Randomizer::getFloat(-100, 100), Randomizer::getFloat(-100, 100), Randomizer::getFloat(-100, 100))*body->getMass());
    };

    // The Z loop must be the first here, because the obstacle deque must be sorted by obstacle' Z coordinate
    for (btScalar z = preciseEdge(playerPosition.Z - buffer); z <= generatedEdgeZ; z += STEP) {

        for (btScalar x = preciseEdge(playerPosition.X - farValueWithBuffer()); x < generatedEdgeLeft; x += STEP)
            for (btScalar y = preciseEdge(playerPosition.Y - farValueWithBuffer());
                y <= preciseEdge(playerPosition.Y + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }

        for (btScalar x = generatedEdgeLeft; x <= generatedEdgeRight; x += STEP) {
            for (btScalar y = preciseEdge(playerPosition.Y - farValueWithBuffer()); y < generatedEdgeBottom; y += STEP)
                handleCell(x, y, z);

            for (btScalar y = generatedEdgeTop + STEP; y <= preciseEdge(playerPosition.Y + farValueWithBuffer()); y += STEP)
                handleCell(x, y, z);
        }

        for (btScalar x = generatedEdgeRight + STEP; x <= preciseEdge(playerPosition.X + farValueWithBuffer()); x += STEP)
            for (btScalar y = preciseEdge(playerPosition.Y - farValueWithBuffer());
                y <= preciseEdge(playerPosition.Y + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }
    }

    for (btScalar z = generatedEdgeZ + STEP; z <= preciseEdge(playerPosition.Z + farValueWithBuffer()); z += STEP)
        for (btScalar x = preciseEdge(playerPosition.X - farValueWithBuffer());
            x <= preciseEdge(playerPosition.X + farValueWithBuffer()); x += STEP)
            for (btScalar y = preciseEdge(playerPosition.Y - farValueWithBuffer());
                y <= preciseEdge(playerPosition.Y + farValueWithBuffer()); y += STEP)
            {
                handleCell(x, y, z);
            }

    #if DEBUG_OUTPUT
    std::cout << obstacleGenerated << " obstacles generated" << std::endl;
    #endif // DEBUG_OUTPUT

    // these are the edges of the generated zone
    // the part of the level behind them is generated and must not be touched
    generatedEdgeLeft = preciseEdge(playerPosition.X - farValueWithBuffer());
    generatedEdgeRight = preciseEdge(playerPosition.X + farValueWithBuffer());
    generatedEdgeBottom = preciseEdge(playerPosition.Y - farValueWithBuffer());
    generatedEdgeTop = preciseEdge(playerPosition.Y + farValueWithBuffer());
    generatedEdgeZ = preciseEdge(playerPosition.Z + farValueWithBuffer());

    // remove obstacles behind the player to save some memory
    removeLeftBehind(playerPosition.Z);
}

btScalar ObstacleGenerator::preciseEdge(btScalar edge) const
{
    return floor(edge / STEP)*STEP;
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
