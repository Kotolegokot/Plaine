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
    Valley(btDynamicsWorld &world, IrrlichtDevice &device, btVector3 position,
           std::size_t cellSize)
    {
        position += { cellSize, cellSize, 0 };
        for (size_t i = 0; i < length; i++) {
            obstacles[i*4] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(-cellSize / 2, 0, cellSize * i), 100);
            obstacles[i*4 + 1] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(cellSize / 2, 0, cellSize * i), 100);
            obstacles[i*4 + 2] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, -cellSize / 2, cellSize * i), 100);
            obstacles[i*4 + 3] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, cellSize / 2,  cellSize * i), 100);
        }
    }

    void getSize(std::size_t &x, std::size_t &y, std::size_t &z) const override
    {
        x = 2;
        y = 2;
        z = length;
    }

    void moveObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) override
    {
        for (auto &obstacleUPtr : obstacles)
            list.push_back(std::move(obstacleUPtr));
    }

    std::size_t getObstacleCount() const override
    {
        return length * 4;
    }

protected:
    std::array<std::unique_ptr<Icosahedron>, length * 4> obstacles;
};

#endif // VALLEY_H
