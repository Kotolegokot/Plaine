#ifndef VALLEY_H
#define VALLEY_H

#include <array>
#include <memory>
#include "obstacles/Icosahedron.h"
#include "IBodyProducer.h"
#include "IObstaclePattern.h"
#include "bodies/IcosahedronProducer.h"
#include "util.h"

template <int Length>
class Alley : public IObstaclePattern
{
public:
    Alley(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { 3, 3, Length * 2 };
    }

    virtual std::vector<std::unique_ptr<IBodyProducer>>
        producers(btVector3 position) const override
    {
        std::vector<std::unique_ptr<IBodyProducer>> result;

        position += { 1.5f * CELL_LENGTH, 1.5f * CELL_LENGTH, 0 };

        constexpr btScalar cos45 = 0.70710678118;
        constexpr btScalar edge = 150;

        for (std::size_t i = 0; i < Length; i++) {
            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativePosition = position + (i % 2 == 0 ?
                        btVector3(-CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                        btVector3(-CELL_LENGTH * cos45, CELL_LENGTH * cos45, CELL_LENGTH * i * 2));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativePosition = position + (i % 2 == 0 ?
                        btVector3(CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                        btVector3(CELL_LENGTH * cos45, -CELL_LENGTH * cos45, CELL_LENGTH * i * 2));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativePosition = position + (i % 2 == 0 ?
                        btVector3(0, -CELL_LENGTH, CELL_LENGTH * i * 2) :
                        btVector3(-CELL_LENGTH * cos45, -CELL_LENGTH * cos45, CELL_LENGTH * i * 2));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativePosition = position + (i % 2 == 0 ?
                        btVector3(0, CELL_LENGTH, CELL_LENGTH * i * 2) :
                        btVector3(CELL_LENGTH * cos45, CELL_LENGTH * cos45, CELL_LENGTH * i * 2));
        }

        return result;
    }

};
#endif // VALLEY_H
