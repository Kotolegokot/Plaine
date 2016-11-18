#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include "obstacles/Box.h"
#include "IObstaclePattern.h"

using namespace irr;

template <int Size>
class Cube : public IObstaclePattern
{
public:
    Cube(int id) :
        IObstaclePattern(id) {}

    Point3<int> size() const override
    {
        return { Size, Size, Size };
    }

    std::size_t produce(btDynamicsWorld &world,
                        IrrlichtDevice &device,
                        btScalar cellSize,
                        btVector3 position,
                        std::list<std::unique_ptr<IObstacle>> &list) const override
    {
        position += { Size * cellSize * 0.5f,
                      Size * cellSize * 0.5f,
                      Size * cellSize * 0.5f };

        auto box = std::make_unique<Box>(world, device, position,
                                         btVector3(Size * cellSize * 0.49,
                                                   Size * cellSize * 0.49,
                                                   Size * cellSize * 0.49 ));

        list.push_back(std::move(box));

        return 1;
    }
};

#endif // CUBE_H
