#ifndef OBSTACLEPATTERNFACTORY_H
#define OBSTACLEPATTERNFACTORY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IObstaclePattern.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Alley.h"


class Patterns {
private:
    static constexpr std::size_t ALL = 6;
    static constexpr std::size_t CRYSTALS = 4;
public:
    Patterns() = delete;

    static const std::array<std::shared_ptr<IObstaclePattern>, ALL> all;
    static const std::array<std::shared_ptr<IObstaclePattern>, CRYSTALS> crystals;
};

#endif // OBSTACLEPATTERNFACTORY_H
