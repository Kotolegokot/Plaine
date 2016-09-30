#include "ObstacleGenerator.h"
#include "util.h"

using namespace irr;

// a little class for cubes
class Cube : public IBody
{
public:
    Cube(btDynamicsWorld *world, IrrlichtDevice *device, btCollisionShape *shape, const btVector3 &position) :
        IBody(world, shape), device(device), position(position)
    {
        createBody();
    }

protected:
    virtual void createNode() override
    {
        node = device->getSceneManager()->addCubeSceneNode(CUBE_SIDE, 0, -1,
            core::vector3df(position.x(), position.y(), position.z()));
        node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        node->setDebugDataVisible(true);
    }

    virtual void createMotionState() override
    {
        motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1), position), node);
    }

    virtual btScalar getMass() override
    {
        return 0.1f;
    }

private:
    IrrlichtDevice *device = nullptr;
    btVector3 position;
};

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, btDynamicsWorld *world, f32 farValue, f32 buffer) :
    device(device), farValue(farValue), world(world), buffer(buffer)
{
    // Bullet
    cubeShape = new btBoxShape(btVector3(CUBE_SIDE / 2, CUBE_SIDE / 2, CUBE_SIDE / 2));
}

ObstacleGenerator::~ObstacleGenerator()
{
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
                bool insideX = false, insideY = false, insideZ = false;
                insideX = x >= generatedEdgeLeft && x <= generatedEdgeRight;
                insideY = y >= generatedEdgeBottom && y <= generatedEdgeTop;
                insideZ = z <= generatedEdgeZ;
                if (!(insideX && insideY && insideZ)) {
                    f32 newX = x + getRandomf(-100, 100);
                    f32 newY = y + getRandomf(-100, 100);
                    f32 newZ = z + getRandomf(-100, 100);

                    Cube *cube = new Cube(world, device, cubeShape, btVector3(newX, newY, newZ));
                    cubes.push_back(cube);
                    cubeCount++;
                }
            }
    generatedEdgeLeft = preciseEdge(playerPosition.X - farValueWithBuffer());
    generatedEdgeRight = preciseEdge(playerPosition.X + farValueWithBuffer());
    generatedEdgeBottom = preciseEdge(playerPosition.Y - farValueWithBuffer());
    generatedEdgeTop = preciseEdge(playerPosition.Y + farValueWithBuffer());
    generatedEdgeZ = preciseEdge(playerPosition.Z + farValueWithBuffer());

    removeLeftBehind(playerPosition.Z);
}

f32 ObstacleGenerator::preciseEdge(f32 edge) const
{
    return edge - (edge - floor(edge / STEP)*STEP);
}

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
