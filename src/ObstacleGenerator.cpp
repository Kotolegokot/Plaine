#include "ObstacleGenerator.h"

ObstacleGenerator::ObstacleGenerator(IrrlichtDevice *device, f32 farValue) :
    device(device), farValue(farValue) {}

void ObstacleGenerator::generate(const core::vector3df &playerPosition)
{
}

void ObstacleGenerator::setFarValue(f32 value)
{
    farValue = value;
}

f32 ObstacleGenerator::getFarValue() const
{
    return farValue;
}
