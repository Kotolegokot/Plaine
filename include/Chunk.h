#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <vector>
#include "ObstaclePatternFactory.h"
#include "Randomizer.h"
#include "util.h"

template <int Size>
class Chunk {
public:
    /* 0     = empty
     * n > 0 = obstacle patterns
     */
    Chunk(const ObstaclePatternFactory &obstaclePatternFactory) :
        factory(obstaclePatternFactory)
    {
        positions.resize(factory.size());
    }

    void generate()
    {
        do {
            for (int i = 0; i < factory.size(); i++)
                positions[i] = createRandomPoint(i);
        } while (collisions());
    }

    // creates objects and returns number of bodies generated
    std::size_t produce(std::list<std::unique_ptr<IObstacle>> &list,
                        btVector3 position, btScalar cellSize)
    {
        std::size_t generated = 0;

        for (int i = 0; i < factory.size(); i++)
            generated += factory[i].produce(position +
                                            btVector3(positions[i].x * cellSize,
                                                      positions[i].y * cellSize,
                                                      positions[i].z * cellSize), list);
        return generated;
    }

private:
    bool collisions()
    {
        Array3<int, Size> data;

        for (int i = 0; i < factory.size(); i++) {
            for (int x = 0; x < factory[i].size().x; x++)
            for (int y = 0; y < factory[i].size().y; y++)
            for (int z = 0; z < factory[i].size().z; z++)
                data.at(positions[i] + Point3<int>(x, y, z)) = i + 1;
        }

        for (int i = 0; i < factory.size(); i++) {
            for (int z = 0; z < factory[i].size().z; z++)
            for (int y = 0; y < factory[i].size().y; y++)
            for (int x = 0; x < factory[i].size().x; x++)
                if (data.at(positions[i] + Point3<int>(x, y, z)) != i + 1)
                    return true;
        }

        return false;
    }

    Point3<int> createRandomPoint(int patternIndex)
    {
        return { Randomizer::getInt(0, Size - factory[patternIndex].size().x),
                 Randomizer::getInt(0, Size - factory[patternIndex].size().y),
                 Randomizer::getInt(0, Size - factory[patternIndex].size().z) };
    }

    std::vector<Point3<int>> positions;
    const ObstaclePatternFactory &factory;
};

#endif //CHUNK_H
