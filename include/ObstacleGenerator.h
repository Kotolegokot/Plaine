#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <deque>
#include <irrlicht.h>

using namespace irr;

class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice *device, f32 farValue = 1500, f32 buffer = 300);
    void generate(const core::vector3df &playerPosition);
    u32 getCubeCount() const;

    void setFarValue(f32 value);
    f32 getFarValue() const;
    void setBuffer(f32 buffer);
    f32 getBuffer() const;

private:
    f32 preciseEdge(f32 edge) const;
    void removeLeftBehind(f32 playerZ);
    f32 farValueWithBuffer() const;

    IrrlichtDevice *device = nullptr;
    std::deque<scene::ISceneNode *> nodes;

    u32 cubeCount = 0;

    f32 farValue = 0;
    f32 buffer = 0;

    f32 generatedEdgeZ = 0;
    f32 generatedEdgeLeft = 0;
    f32 generatedEdgeRight = 0;
    f32 generatedEdgeTop = 0;
    f32 generatedEdgeBottom = 0;

    const f32 STEP = 400;
};

#endif // OBSTACLEGENERATOR_H
