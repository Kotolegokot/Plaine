#ifndef VALLEY_H
#define VALLEY_H

#include <array>
#include <memory>
#include "obstacles/Icosahedron.h"
#include "IObstacle.h"
#include "IObstaclePattern.h"

template <int Length>
class Alley : public IObstaclePattern
{
public:
    Alley(int id) :
        IObstaclePattern(id) {}

    Point3<int> size() const override
    {
        return { 3, 3, Length * 2 };
    }

    virtual std::size_t produce(btDynamicsWorld &world,
                                IrrlichtDevice &device,
                                btScalar cellSize,
                                btVector3 position,
                                std::list<std::unique_ptr<IObstacle>> &list) const override
    {
        position += { 1.5f * cellSize, 1.5f * cellSize, 0 };

        constexpr btScalar cos45 = 0.70710678118;
        constexpr btScalar radius = 150;

        for (std::size_t i = 0; i < Length; i++) {
            auto obstacle1 =
                    std::make_unique<Icosahedron>(world, device, position
                        + (i % 2 == 0 ?
                            btVector3(-cellSize, 0, cellSize * i * 2) :
                            btVector3(-cellSize * cos45, cellSize * cos45, cellSize * i * 2)),
                        radius);

            auto obstacle2 =
                    std::make_unique<Icosahedron>(world, device, position
                        + (i % 2 == 0 ?
                            btVector3(cellSize, 0, cellSize * i * 2) :
                            btVector3(cellSize * cos45, -cellSize * cos45, cellSize * i * 2)),
                        radius);

            auto obstacle3 =
                    std::make_unique<Icosahedron>(world, device, position
                        + (i % 2 == 0 ?
                            btVector3(0, -cellSize, cellSize * i * 2) :
                            btVector3(-cellSize * cos45, -cellSize * cos45, cellSize * i * 2)),
                        radius);

            auto obstacle4 =
                    std::make_unique<Icosahedron>(world, device, position
                        + (i % 2 == 0 ?
                            btVector3(0, cellSize, cellSize * i * 2) :
                            btVector3(cellSize * cos45, cellSize * cos45, cellSize * i * 2)),
                        radius);

            list.push_back(std::move(obstacle1));
            list.push_back(std::move(obstacle2));
            list.push_back(std::move(obstacle3));
            list.push_back(std::move(obstacle4));
        }

        return Length * 4;
    }
};
#endif // VALLEY_H
