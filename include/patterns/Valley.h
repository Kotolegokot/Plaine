#ifndef VALLEY_H
#define VALLEY_H

#include <array>
#include <memory>
#include "obstacles/Icosahedron.h"
#include "IObstacle.h"
#include "IObstaclePattern.h"

template <size_t length>
class Valley : public IObstaclePattern
{
public:
    Valley(btDynamicsWorld &world, IrrlichtDevice &device, const btVector3 &position,
           btScalar width) :
        IObstaclePattern(world, device, position), width(width)
    {
        for (size_t i = 0; i < length; i++) {
            obstacles[i*4] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(-width / 2, 0, STEP * i), 100);
            obstacles[i*4 + 1] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(width / 2, 0, STEP * i), 100);
            obstacles[i*4 + 2] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, -width / 2, STEP * i), 100);
            obstacles[i*4 + 3] =
                    std::make_unique<Icosahedron>(world, device, position
                                                  + btVector3(0, width / 2,  STEP * i), 100);

        }
    }

    void addObstaclesToList(std::list<std::unique_ptr<IObstacle>> &list) override
    {
        for (size_t i = 0; i < obstacles.size(); i++)
            list.push_back(std::move(obstacles[i]));
    }

    size_t getObstacleCount() const override
    {
        return length * 4;
    }

private:
    static constexpr btScalar STEP = 200;
    btScalar width = 0;

    std::array<std::unique_ptr<Icosahedron>, length * 4> obstacles;
};

#endif // VALLEY_H
