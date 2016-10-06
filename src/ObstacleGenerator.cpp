#include "ObstacleGenerator.h"
#include "util.h"

using namespace irr;

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, btDynamicsWorld *world, f32 farValue, f32 buffer) :
    device(device), farValue(farValue), buffer(buffer), world(world) {}

ObstacleGenerator::~ObstacleGenerator()
{
    // remove all stored cubes
    while (!cubes.empty()) {
        delete cubes.front();
        cubes.pop_front();
    }
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
#ifdef REMOVE_CENTER
                inside = inside || (x >= -500 && x <= 500);
#endif

                // if it's not, we create a new cube
                if (!inside) {
                    // add some randomness to its position (for fun ofc)
                    f32 newX = x + getRandomf(-100, 100);
                    f32 newY = y + getRandomf(-100, 100);
                    f32 newZ = z + getRandomf(-100, 100);

                    // create the cube and add it to the deque
                    if(std::abs(newX) > 400) {
                        IBody *body = nullptr;
                        //body = new Cube(world, device, btVector3(newX, newY, newZ), getRandomf(50.0f, 250.0f));
                        body = new Torus(world, device, btVector3(newX, newY, newZ), getRandomf(50.f, 250.f));

                        if (int(getRandomf(1, 8)) == 1)
                            body->getRigidBody()->applyTorqueImpulse(btVector3(getRandomf(-10000, 10000), getRandomf(-10000, 10000), getRandomf(-10000, 10000))*body->getMass());
                        if (int(getRandomf(1, 8)) == 1)
                            body->getRigidBody()->applyCentralImpulse(btVector3(getRandomf(-100, 100), getRandomf(-100, 100), getRandomf(-100, 100))*body->getMass());
                        cubes.push_back(body);
                        cubeCount++;
                    }
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
    return edge - (edge - floor(edge / STEP)*STEP);
}

// removes obstacles behind the player
void ObstacleGenerator::removeLeftBehind(f32 playerZ)
{
    while (!cubes.empty()) {
        if (cubes.front()->getNode()->getPosition().Z < playerZ - buffer)
        {
            delete cubes.front();
            cubes.pop_front();
            cubeCount--;
        } else
            break;
    }
}

u32 ObstacleGenerator::getCubeCount() const
{
    return cubeCount;
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
