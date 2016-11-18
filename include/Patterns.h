#ifndef PATTERNS_H
#define PATTERNS_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IObstaclePattern.h"
#include "patterns/Cube.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Alley.h"


class Patterns {
private:
    static constexpr std::size_t CRYSTALS = 4;
    static constexpr std::size_t CUBES = 2;

    static constexpr std::size_t ALL = CRYSTALS + CUBES + 2;
public:
    Patterns() = delete;

    static const std::array<std::shared_ptr<IObstaclePattern>, ALL> all;
    static const std::array<std::shared_ptr<IObstaclePattern>, CRYSTALS> crystals;
    static const std::array<std::shared_ptr<IObstaclePattern>, CUBES> cubes;
};

#endif // PATTERNS_H
