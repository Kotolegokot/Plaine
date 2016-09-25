#include "ObstacleGenerator.h"

using namespace irr;

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, f32 farValue) :
    device(device), farValue(farValue) {}

void ObstacleGenerator::generate(const core::vector3df &playerPosition)
{
    // The Z loop must be the first here, because the "nodes" deque must be sorted by nodes' Z coordinate
    for (f32 z = preciseEdge(playerPosition.Z); z < preciseEdge(playerPosition.Z + farValue); z += STEP)
        for (f32 x = preciseEdge(playerPosition.X - farValue); x < preciseEdge(playerPosition.X + farValue); x += STEP)
            for (f32 y = preciseEdge(playerPosition.Y - farValue); y < preciseEdge(playerPosition.Y + farValue); y += STEP) {
                bool insideX = false, insideY = false, insideZ = false;
                insideX = x > generatedEdgeLeft && x < generatedEdgeRight;
                insideY = y > generatedEdgeBottom && y < generatedEdgeTop;
                insideZ = z < generatedEdgeZ;
                if (!(insideX && insideY && insideZ)) {
                    scene::ISceneManager *sceneManager = device->getSceneManager();
                    scene::ISceneNode *node = sceneManager->addCubeSceneNode(50.f, 0, -1, core::vector3df(x, y, z));
                    node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/textures/lsd.png"));

                    nodes.push_back(node);
                }
            }

    generatedEdgeLeft = preciseEdge(playerPosition.X - farValue);
    generatedEdgeRight = preciseEdge(playerPosition.X + farValue);
    generatedEdgeBottom = preciseEdge(playerPosition.Y - farValue);
    generatedEdgeTop = preciseEdge(playerPosition.Y + farValue);
    generatedEdgeZ = preciseEdge(playerPosition.Z + farValue);

    removeLeftBehind(playerPosition.Z);
}

f32 ObstacleGenerator::preciseEdge(f32 edge) const
{
    return edge - (edge - floor(edge / STEP)*STEP);
}

void ObstacleGenerator::removeLeftBehind(f32 playerZ)
{
    while (!nodes.empty())
        if (nodes.front()->getPosition().Z < playerZ) {
            nodes.front()->remove();
            nodes.pop_front();
        } else {
            break;
        }
}

void ObstacleGenerator::setFarValue(f32 value)
{
    farValue = value;
}

f32 ObstacleGenerator::getFarValue() const
{
    return farValue;
}
