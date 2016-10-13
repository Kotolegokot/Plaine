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

using namespace irr;

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, btDynamicsWorld *world, f32 farValue, f32 buffer) :
    device(device), farValue(farValue), buffer(buffer), world(world) {}

ObstacleGenerator::~ObstacleGenerator()
{
    // remove all stored cubes
    while (!obstacles.empty())
        obstacles.pop_front();
}

void ObstacleGenerator::generate(const core::vector3df &playerPosition)
{
    // The Z loop must be the first here, because the "nodes" deque must be sorted by nodes' Z coordinate
    for (f32 z = preciseEdge(playerPosition.Z); z <= preciseEdge(playerPosition.Z + farValueWithBuffer()); z += STEP)
        for (f32 x = preciseEdge(playerPosition.X - farValueWithBuffer()); x <= preciseEdge(playerPosition.X + farValueWithBuffer()); x += STEP)
            for (f32 y = preciseEdge(playerPosition.Y - farValueWithBuffer()); y <= preciseEdge(playerPosition.Y + farValueWithBuffer()); y += STEP) {

                // check if the position is within the already generated zone
                bool insideX = false, insideY = false, insideZ = false;
                insideX = x >= generatedEdgeLeft && x <= generatedEdgeRight;
                insideY = y >= generatedEdgeBottom && y <= generatedEdgeTop;
                insideZ = z <= generatedEdgeZ;

                bool inside = insideX && insideY && insideZ;

                // if it's not, we create a new cube
                if (!inside) {
                    // add some randomness to its position (for fun ofc)
                    f32 newX = x + getRandomf(-100, 100);
                    f32 newY = y + getRandomf(-100, 100);
                    f32 newZ = z + getRandomf(-100, 100);

                    // create obstacles and add them to the deque
                    switch(int(getRandomf(0, 6))){
                    case 0:
                        {
                            IObstaclePattern *pattern = nullptr;
                            pattern = new Tunnel(world, device, btVector3(newX, newY, newZ), getRandomf(100, 200), getRandomf(300, 600));
                            pattern->addObstaclesToDeque(obstacles);
                            obstacleCount += pattern->getObstacleCount();
                            delete pattern;
                            break;
                        }
                    case 1:
                        {
                            IObstaclePattern *pattern = nullptr;
                            pattern = new Crystal(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 250.f), getRandomf(300.f, 600.f));
                            pattern->addObstaclesToDeque(obstacles);
                            obstacleCount += pattern->getObstacleCount();
                            delete pattern;
                            break;
                        }
                    case 2:
                        {
                            std::unique_ptr<IObstacle> obstacle =
                                std::make_unique<Box>(world, device, btVector3(newX, newY, newZ),
                                    btVector3(getRandomf(50.0f, 250.0f), getRandomf(50.f, 250.f), getRandomf(50.f, 250.f)));
                            obstacles.push_back(std::move(obstacle));
                            obstacleCount++;
                            break;
                        }
                        case 3:
                        {
                            std::unique_ptr<IObstacle> obstacle =
                                std::make_unique<Icosahedron>(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 250.f));
                            obstacles.push_back(std::move(obstacle));
                            obstacleCount++;
                            break;
                        }
                    case 4:
                        {
                            std::unique_ptr<IObstacle> obstacle =
                                std::make_unique<Icosphere2>(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 200.f));
                            obstacles.push_back(std::move(obstacle));
                            obstacleCount++;
                            break;
                        }
                    case 5:
                        {
                            std::unique_ptr<IObstacle> obstacle =
                                std::make_unique<Tetrahedron>(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 250.f));
                            obstacles.push_back(std::move(obstacle));
                            obstacleCount++;
                            break;
                        }
                    case 6:
                        {
                            std::unique_ptr<IObstacle> obstacle =
                                std::make_unique<Cone>(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 200.f), getRandomf(50.f, 200.f));
                            obstacles.push_back(std::move(obstacle));
                            obstacleCount++;
                            break;
                        }
                    }

                    //if (int(getRandomf(1, 1)) == 1)
                    //    body->getRigidBody()->applyTorqueImpulse(btVector3(getRandomf(-10000, 10000), getRandomf(-10000, 10000), getRandomf(-10000, 10000))*body->getMass());
                    //if (int(getRandomf(1, 1)) == 1)
                    //    body->getRigidBody()->applyCentralImpulse(btVector3(getRandomf(-100, 100), getRandomf(-100, 100), getRandomf(-100, 100))*body->getMass());
                }
            }

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

f32 ObstacleGenerator::preciseEdge(f32 edge) const
{
    return floor(edge / STEP)*STEP;
}

// removes obstacles behind the player
void ObstacleGenerator::removeLeftBehind(f32 playerZ)
{
    while (!obstacles.empty()) {
        if (obstacles.front()->getPosition().z() < playerZ - buffer)
        {
            obstacles.pop_front();
            obstacleCount--;
        } else
            break;
    }
}

u32 ObstacleGenerator::getCubeCount() const
{
    return obstacleCount;
}

f32 ObstacleGenerator::farValueWithBuffer() const
{
    return farValue + buffer;
}

void ObstacleGenerator::setFarValue(f32 value)
{
    farValue = value;
}

f32 ObstacleGenerator::getFarValue() const
{
    return farValue;
}

void ObstacleGenerator::setBuffer(f32 buffer)
{
    this->buffer = buffer;
}

f32 ObstacleGenerator::getBuffer() const
{
    return buffer;
}
