#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include "IBodyProducer.h"
#include "IObstaclePattern.h"
#include "bodies/BoxProducer.h"
#include "util.h"

using namespace irr;

template <int Size>
class Cube : public IObstaclePattern
{
public:
    Cube(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { Size, Size, Size };
    }

    std::vector<std::unique_ptr<IBodyProducer>>
        producers() const override
    {
        btVector3 position { Size * CELL_LENGTH * 0.5f,
                             Size * CELL_LENGTH * 0.5f,
                             Size * CELL_LENGTH * 0.5f };

        std::vector<std::unique_ptr<IBodyProducer>> result;
        result.push_back(std::make_unique<BoxProducer>(btVector3(1, 1, 1) *
                                                       Size * CELL_LENGTH * 0.49));
        result.back()->relativeTransform.setOrigin(position);
        result.back()->relativeTransform.setRotation(btQuaternion(0, 0, PI<btScalar> * 0.5f));

        return result;
    }
};

#endif // CUBE_H
