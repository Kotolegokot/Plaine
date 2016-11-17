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
    Chunk() = default;

    void generate()
    {
        do {
            positions.resize(Randomizer::getInt(0, 10));

            for (std::size_t i = 0; i < positions.size(); i++)
                positions[i] = createRandomPosition();
        } while (collisions() != positions.size());
    }

    // creates objects and returns number of bodies generated
    std::size_t produce(btDynamicsWorld &world,
                        IrrlichtDevice &device,
                        btScalar cellSize,
                        btVector3 chunkPosition,
                        std::list<std::unique_ptr<IObstacle>> &list) const
    {
        std::size_t generated = 0;

        for (std::size_t i = 0; i < positions.size(); i++) {
            const int patternIndex = positions[i].first;
            const Point3<int> &pos = positions[i].second;

            generated += ObstaclePatternFactory::at(patternIndex).produce(world, device, cellSize,
                        chunkPosition + btVector3(pos.x, pos.y, pos.z) * cellSize, list);
        }
        return generated;
    }

private:
    // returns how many obstacles it managed to fit into the chunk
    std::size_t collisions()
    {
        std::size_t result = 0;
        /* 0     = empty
         * n > 0 = obstacle patterns
         */
        Array3<int, Size> data;
        data.fill(0);

        for (std::size_t i = 0; i < positions.size(); i++) {
            const int patternIndex = positions[i].first;
            const Point3<int> &pos = positions[i].second;

            for (int x = 0; x < ObstaclePatternFactory::at(patternIndex).size().x; x++)
            for (int y = 0; y < ObstaclePatternFactory::at(patternIndex).size().y; y++)
            for (int z = 0; z < ObstaclePatternFactory::at(patternIndex).size().z; z++) {
                int &currentCell = data.at(pos + Point3<int>(x, y, z));

                if (currentCell != 0)
                    return result;
                else
                    data.at(pos + Point3<int>(x, y, z)) = i + 1;
            }

            result++;
        }

        return result;
    }

    static PatternPosition createRandomPosition()
    {
        int patternIndex = Randomizer::getInt(0, ObstaclePatternFactory::size() - 1);
        Point3<int> position = { Randomizer::getInt(0, Size - ObstaclePatternFactory::at(patternIndex).size().x),
                                 Randomizer::getInt(0, Size - ObstaclePatternFactory::at(patternIndex).size().y),
                                 Randomizer::getInt(0, Size - ObstaclePatternFactory::at(patternIndex).size().z) };

        return { patternIndex, position };
    }

    std::vector<PatternPosition> positions;
};

#endif //CHUNK_H
