#include "ObstacleGenerator.h"
#include "util.h"

#include "IBody.h"

using namespace irr;

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, btDynamicsWorld *world, f32 farValue, f32 buffer) :
    device(device), world(world), farValue(farValue), buffer(buffer)
{
    // Bullet
    cubeShape = new btBoxShape(btVector3(CUBE_SIDE / 2, CUBE_SIDE / 2, CUBE_SIDE / 2));
}

ObstacleGenerator::~ObstacleGenerator()
{
    while (!cubes.empty()) {
        world->removeRigidBody(cubes.front());
        delete cubes.front()->getMotionState();
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

                    cubes.push_back(createCube(core::vector3df(newX, newY, newZ)));
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

btRigidBody *ObstacleGenerator::createCube(const core::vector3df &position)
{
    const btScalar mass = 0.1;

    scene::ISceneNode *cubeNode = device->getSceneManager()->addCubeSceneNode(CUBE_SIDE, 0, -1, position);
    cubeNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));
    cubeNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	cubeNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
	cubeNode->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
	cubeNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
	cubeNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	cubeNode->setDebugDataVisible(true);

    btMotionState *motionState = new MotionState(btTransform(btQuaternion(0, 0, 0, 1),
        btVector3(position.X, position.Y, position.Z)), cubeNode);
    btVector3 cubeInertia(0, 0, 0);
    cubeShape->calculateLocalInertia(mass, cubeInertia);
    btRigidBody::btRigidBodyConstructionInfo cubeCI(mass,  motionState, cubeShape, cubeInertia);
    btRigidBody *cubeBody = new btRigidBody(cubeCI);
    world->addRigidBody(cubeBody);

    return cubeBody;
}

f32 ObstacleGenerator::preciseEdge(f32 edge) const
{
    return edge - (edge - floor(edge / STEP)*STEP);
}

void ObstacleGenerator::removeLeftBehind(f32 playerZ)
{
    while (!cubes.empty()) {
        btTransform transform;
        cubes.front()->getMotionState()->getWorldTransform(transform);
        if (transform.getOrigin().z() < playerZ - buffer) {
            world->removeRigidBody(cubes.front());
            delete cubes.front()->getMotionState();
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
