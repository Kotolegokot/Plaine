#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <vector>
#include "ObstaclePatternFactory.h"
#include "Randomizer.h"
#include "util.h"

template <int Size>
class Chunk {
private:
    using PatternPosition = std::pair<int, Point3<int>>;
public:
    Chunk(const ObstaclePatternFactory &obstaclePatternFactory) :
        factory(obstaclePatternFactory)
    {}

    void generate()
    {
        do {
            positions.resize(Randomizer::getInt(0, 10));

            for (std::size_t i = 0; i < positions.size(); i++)
                positions[i] = createRandomPosition();
        } while (collisions());
    }

    // creates objects and returns number of bodies generated
    std::size_t produce(std::list<std::unique_ptr<IObstacle>> &list,
                        btVector3 chunkPosition, btScalar cellSize)
    {
        std::size_t generated = 0;

        for (std::size_t i = 0; i < positions.size(); i++) {
            int patternIndex = positions[i].first;
            Point3<int> &pos = positions[i].second;

            generated += factory[patternIndex].produce(
                        chunkPosition + btVector3(pos.x, pos.y, pos.z) * cellSize, list);
        }
        return generated;
    }

private:
    bool collisions()
    {
        /* 0     = empty
         * n > 0 = obstacle patterns
         */
        Array3<int, Size> data;
        data.fill(0);

        for (std::size_t i = 0; i < positions.size(); i++) {
            int patternIndex = positions[i].first;
            Point3<int> &pos = positions[i].second;

            for (int x = 0; x < factory[patternIndex].size().x; x++)
            for (int y = 0; y < factory[patternIndex].size().y; y++)
            for (int z = 0; z < factory[patternIndex].size().z; z++) {
                int &currentCell = data.at(pos + Point3<int>(x, y, z));

                if (currentCell != 0)
                    return true;
                else
                    data.at(pos + Point3<int>(x, y, z)) = i + 1;
            }
        }

        return false;
    }

    PatternPosition createRandomPosition()
    {
        int patternIndex = Randomizer::getInt(0, factory.size() - 1);
        Point3<int> position = { Randomizer::getInt(0, Size - factory[patternIndex].size().x),
                                 Randomizer::getInt(0, Size - factory[patternIndex].size().y),
                                 Randomizer::getInt(0, Size - factory[patternIndex].size().z) };

        return { patternIndex, position };
    }

    std::vector<PatternPosition> positions;
    const ObstaclePatternFactory &factory;
};

#endif //CHUNK_H
