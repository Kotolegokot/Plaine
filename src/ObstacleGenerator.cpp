#include "ObstacleGenerator.h"
#include "util.h"

#define MASS_COEFFICIENT 500000000

using namespace irr;

// a little class for cubes
class Cube : public IBody
{
public:
    Cube(btDynamicsWorld *world, IrrlichtDevice *device, const btVector3 &position, const f32 &side) :
        IBody(world), device(device), position(position), side(side)
    {
        createBody();
    }

protected:
    virtual void createNode() override
    {
        node = device->getSceneManager()->addCubeSceneNode(side, 0, -1,
            core::vector3df(position.x(), position.y(), position.z()));
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));

        // just for cubes' beauty
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    }

    virtual void createMotionState() override
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual void createShape() override
    {
        // create shape for cubes
        shape = new btBoxShape(btVector3(side/2, side/2, side/2));
    }

    virtual btScalar getMass() override
    {
        return side*side*side/MASS_COEFFICIENT;
    }

private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
    f32 side;
};

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

                // if it's not, we create a new cube
                if (!(insideX && insideY && insideZ)) {
                    // add some randomness to its position (for fun ofc)
                    f32 newX = x + getRandomf(-100, 100);
                    f32 newY = y + getRandomf(-100, 100);
                    f32 newZ = z + getRandomf(-100, 100);

                    // create the cube and add it to the deque
                    Cube *cube = new Cube(world, device, btVector3(newX, newY, newZ), getRandomf(50.0f, 300.0f));
                    cubes.push_back(cube);
                    cubeCount++;
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
        btTransform transform;
        cubes.front()->getRigidBody()->getMotionState()->getWorldTransform(transform);
        if (transform.getOrigin().z() < playerZ - buffer) {
            delete cubes.front();
            cubes.pop_front();
            cubeCount--;
        } else {
            break;
        }
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
