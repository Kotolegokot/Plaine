#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <deque>
#include <irrlicht.h>

using namespace irr;

class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice *device, f32 farValue = 300);
    void generate(const core::vector3df &playerPosition);
    void setFarValue(f32 value);
    f32 getFarValue() const;

private:
    f32 preciseEdge(f32 edge) const;
    void removeLeftBehind(f32 playerZ);

    IrrlichtDevice *device = nullptr;
    std::deque<scene::ISceneNode *> nodes;

    f32 farValue = 0;
    f32 generatedEdgeZ = 0;
    f32 generatedEdgeLeft = 0;
    f32 generatedEdgeRight = 0;
    f32 generatedEdgeTop = 0;
    f32 generatedEdgeBottom = 0;

    const f32 STEP = 200;
    const f32 BUFFER = 200;
};

#endif // OBSTACLEGENERATOR_H
