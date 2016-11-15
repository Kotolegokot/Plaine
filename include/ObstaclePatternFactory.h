#ifndef OBSTACLEPATTERNFACTORY_H
#define OBSTACLEPATTERNFACTORY_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IObstaclePattern.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Valley.h"

// it uses int in interface functions for simplicity
//   and because we are quite sure there cannot be more than std::numeric_limits<int>::max()
//   patterns in the factory
class ObstaclePatternFactory {
public:
    ObstaclePatternFactory(btDynamicsWorld &world, IrrlichtDevice &device, btScalar cellSize)
    {
        patterns[0] = std::make_unique<Crystal>(world, device, cellSize);
        patterns[1] = std::make_unique<Tunnel>(world, device, cellSize);
        patterns[2] = std::make_unique<Valley<5>>(world, device, cellSize);
    }

    IObstaclePattern &operator [](int index)
    {
        return *patterns[static_cast<std::size_t>(index)];
    }

    const IObstaclePattern &operator [](int index) const
    {
        return *patterns[static_cast<std::size_t>(index)];
    }

    int size() const
    {
        return static_cast<int>(patterns.size());
    }

private:
    std::array<std::unique_ptr<IObstaclePattern>, 3> patterns;
};

#endif // OBSTACLEPATTERNFACTORY_H
