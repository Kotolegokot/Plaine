#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <vector>
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
    IrrlichtDevice *device = nullptr;
    f32 farValue = 0;
    f32 generatedEdgeZ = 0;
    f32 generatedEdgeLeft = 0;
    f32 generatedEdgeRight = 0;
};

#endif // OBSTACLEGENERATOR_H
