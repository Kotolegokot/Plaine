#ifndef VALLEY_H
#define VALLEY_H

#include <array>
#include <memory>
#include "obstacles/Icosahedron.h"
#include "IObstacle.h"
#include "IObstaclePattern.h"

template <std::size_t length>
class Valley : public IObstaclePattern
{
public:
    Valley(btDynamicsWorld &world, IrrlichtDevice &device, btScalar cellSize) :
        IObstaclePattern(world, device, cellSize)
    {}

    std::tuple<std::size_t, std::size_t, std::size_t> getSize() const override
    {
        return { 2, 2, length };
    }

    std::size_t produce(btVector3 position, std::list<std::unique_ptr<IObstacle> > &list) const override
    {
        position += { cellSize, cellSize, 0 };

        for (std::size_t i = 0; i < length; i++) {
            auto obstacle1 =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(-cellSize / 2, 0, cellSize * i), 100);
            auto obstacle2 =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(cellSize / 2, 0, cellSize * i), 100);
            auto obstacle3 =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, -cellSize / 2, cellSize * i), 100);
            auto obstacle4 =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, cellSize / 2,  cellSize * i), 100);

            list.push_back(std::move(obstacle1));
            list.push_back(std::move(obstacle2));
            list.push_back(std::move(obstacle3));
            list.push_back(std::move(obstacle4));
        }

        return length * 4;
    }
};
#endif // VALLEY_H
