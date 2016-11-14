#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include "ObstaclePatternFactory.h"

template <std::size_t Size>
class Chunk {
public:
    /* 0     = empty
     * 1     = occupied
     * n > 1 = obstacle patterns
     */
    Chunk(const ObstaclePatternFactory &obstaclePatternFactory) :
        obstaclePatternFactory(obstaclePatternFactory)
    {
        clear();
    }

    void clear()
    {
        data.fill(0);
    }

    void generate()
    {
        clear();
    }

    // creates objects and returns number of bodies generated
    std::size_t produce(std::list<std::unique_ptr<IObstacle>> &list,
                        btVector3 position, btScalar cellSize)
    {
        std::size_t generated = 0;

        for (std::size_t x = 0; x < Size; x++)
            for (std::size_t y = 0; y < Size; y++)
                for (std::size_t z = 0; z < Size; z++)
                    if (this->get(x, y, z) > 1)
                        generated += obstaclePatternFactory[this->get(x, y, z) - 2].produce(
                                position + btVector3(x * cellSize, y * cellSize, z * cellSize),
                                list);

        return generated;
    }

private:
    std::size_t &get(std::size_t x, std::size_t y, std::size_t z)
    {
        return data[x + y * Size + z * Size * Size];
    }

    std::array<std::size_t, Size * Size * Size> data;
    const ObstaclePatternFactory &obstaclePatternFactory;
};

#endif //CHUNK_H
