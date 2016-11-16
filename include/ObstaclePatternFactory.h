#ifndef OBSTACLEPATTERNFACTORY_H
#define OBSTACLEPATTERNFACTORY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IObstaclePattern.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Valley.h"

class ObstaclePatternFactory {
private:
    static constexpr std::size_t PATTERNS_COUNT = 3;
public:
    ObstaclePatternFactory() = delete;

    static const IObstaclePattern &at(std::size_t index)
    {
        return *patterns[index];
    }

    static std::size_t size()
    {
        return patterns.size();
    }

private:
    static const std::array<std::unique_ptr<IObstaclePattern>, PATTERNS_COUNT> patterns;
};

#endif // OBSTACLEPATTERNFACTORY_H
