#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <vector>
#include "Patterns.h"
#include "Randomizer.h"
#include "util.h"

template <int Size>
class Chunk {
private:
    struct PatternPosition {
        PatternPosition() = default;
        PatternPosition(std::shared_ptr<IObstaclePattern> pattern,
                        Vector3<int> position) :
            pattern(pattern), position(position) {}
        PatternPosition(const PatternPosition &) = default;
        PatternPosition &operator =(const PatternPosition &) = default;
        PatternPosition(PatternPosition &&) noexcept = default;
        PatternPosition &operator =(PatternPosition &&) noexcept = default;

        std::shared_ptr<IObstaclePattern> pattern;
        Vector3<int> position;
    };

    enum class ChunkType { CLOUD, RANDOM, NOT_GENERATED };
public:
    Chunk() = default;

    void generate()
    {
        switch (Randomizer::getInt(0, 2)) {
        case 0: { // cloud of crystals
            std::size_t n;
            do {
                positions.clear();

                positions.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int patternIndex = Randomizer::getInt(0, Patterns::crystals.size() - 1);

                    positions.push_back(randomPosition(Patterns::crystals[patternIndex]));
                }
            } while ((n = collisions()) < Size * Size / 8);
            positions.resize(n);
            type = ChunkType::CLOUD;

            break;
        }

        case 1: { // cloud of cubes
            std::size_t n;
            do {
                positions.clear();

                positions.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int patternIndex = Randomizer::getInt(0, Patterns::cubes.size() - 1);

                    positions.push_back(randomPosition(Patterns::cubes[patternIndex]));
                }
            } while ((n = collisions()) < Size * Size / 8);
            positions.resize(n);
            type = ChunkType::CLOUD;

            break;
        }

        case 2: // absolutely random chunk
            do {
                positions.clear();

                const std::size_t count = Randomizer::getInt(5, 10);
                positions.reserve(count);
                for (std::size_t i = 0; i < count; i++) {
                    const int patternIndex = Randomizer::getInt(0, Patterns::all.size() - 1);

                    positions.push_back(randomPosition(Patterns::all[patternIndex]));
                }
            } while (collisions() != positions.size());
            type = ChunkType::RANDOM;

            break;

        default:
            break;
        }
    }

    // creates objects and returns number of bodies generated
    std::size_t produce(btDynamicsWorld &world,
                        IrrlichtDevice &device,
                        btScalar cellSize,
                        btVector3 chunkPosition,
                        std::list<std::unique_ptr<IObstacle>> &list) const
    {
        if (type == ChunkType::NOT_GENERATED)
            return 0;

        std::size_t generated = 0;

        for (const auto &position : positions) {
            const auto &pattern = position.pattern;
            const Vector3<int> &pos = position.position;

            generated += pattern->produce(world, device, cellSize,
                        chunkPosition + btVector3(pos.x, pos.y, pos.z) * cellSize, list);
        }
        return generated;
    }

    // creates objects and returns number of bodies generated
    std::size_t produceCell(btDynamicsWorld &world,
                            IrrlichtDevice &device,
                            btScalar cellSize,
                            btVector3 chunkPosition,
                            std::list<std::unique_ptr<IObstacle>> &list,
                            Vector3<int> cell) const
    {
        if (type == ChunkType::NOT_GENERATED)
            return 0;

        std::size_t generated = 0;

        for (const auto &position : positions) {
            const auto &pattern = position.pattern;
            const Vector3<int> &pos = position.position;

            if (pos == cell)
                return generated += pattern->produce(world, device, cellSize,
                    chunkPosition + pos.toBulletVector3() * cellSize, list);
        }

        return 0;
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

        for (const auto &position : positions) {
            const auto &pattern = position.pattern;
            const Vector3<int> &pos = position.position;

            for (int x = 0; x < pattern->size().x; x++)
            for (int y = 0; y < pattern->size().y; y++)
            for (int z = 0; z < pattern->size().z; z++) {
                int &currentCell = data.at(pos + Vector3<int>(x, y, z));

                if (currentCell != 0)
                    return result;
                else
                    data.at(pos + Vector3<int>(x, y, z)) = pattern->id() + 1;
            }

            result++;
        }

        return result;
    }

    static PatternPosition randomPosition(std::shared_ptr<IObstaclePattern> pattern)
    {
        return { pattern,
                 { Randomizer::getInt(0, Size - pattern->size().x),
                   Randomizer::getInt(0, Size - pattern->size().y),
                   Randomizer::getInt(0, Size - pattern->size().z) } };
    }

    std::vector<PatternPosition> positions;
    ChunkType type = ChunkType::NOT_GENERATED;
};

#endif //CHUNK_H
