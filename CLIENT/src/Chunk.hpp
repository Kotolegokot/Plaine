/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <vector>
#include <algorithm>
#include "Patterns.hpp"
#include "util/Randomizer.hpp"
#include "util/other.hpp"
#include "util/Array3.hpp"

template <int Size>
class Chunk {
private:
    struct PatternPosition {
        PatternPosition() = default;
        PatternPosition(std::shared_ptr<IObstaclePattern> pattern, Vector3<int> position) :
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
        std::vector<PatternPosition> positions;

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
            } while ((n = collisions(positions)) < Size * Size / 8);
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
            } while ((n = collisions(positions)) < Size * Size / 8);
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
            } while (collisions(positions) != positions.size());
            type = ChunkType::RANDOM;

            break;

        default:
            break;
        }

        generateMap(positions);
    }

    // creates objects and returns number of bodies generated
    std::size_t produceCell(btDynamicsWorld &world,
                            IrrlichtDevice &device,
                            btVector3 chunk,
                            Vector3<int> cell,
                            std::list<std::unique_ptr<Body>> &list) const
    {
        if (type == ChunkType::NOT_GENERATED)
            return 0;

        std::size_t generated = 0;

        for (const auto &producer : map[cell]) {
            auto body = producer->produce(world, device, chunk + cell * CELL_LENGTH);
            list.push_back(std::move(body));

            generated++;
        }

        return generated;
    }

private:
    // returns how many obstacles it managed to fit into the chunk
    std::size_t collisions(const std::vector<PatternPosition> &positions)
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

    void generateMap(const std::vector<PatternPosition> &positions)
    {
        // clear the map
        std::for_each(map.begin(), map.end(), [](auto &vector) { vector.clear(); });

        for (const auto &position : positions) {
            const auto &pattern = *position.pattern;
            const Vector3<int> &pos = position.position;

            for (auto &producer : pattern.producers()) {
                // producer pos relative to pattern
                btVector3 &origin = producer->relativeTransform.getOrigin();
                origin += pos * CELL_LENGTH;

                // producer cell relative to chunk
                const Vector3<int> cell = (Vector3<int>(origin) / CELL_LENGTH);

                // producer pos relative to cell
                origin -= cell * CELL_LENGTH;

                map[cell].push_back(std::move(producer));
            }
        }
    }

    static Vector3<int> cellPos(const btVector3 &position)
    {
        return Vector3<int>(position) / CELL_LENGTH;
    }

    static btVector3 cellRelativePosition(const btVector3 &position, const Vector3<int> &cell)
    {
        return position - cell * CELL_LENGTH;
    }

    static PatternPosition randomPosition(std::shared_ptr<IObstaclePattern> pattern)
    {
        return { pattern,
                 { Randomizer::getInt(0, Size - pattern->size().x),
                   Randomizer::getInt(0, Size - pattern->size().y),
                   Randomizer::getInt(0, Size - pattern->size().z) } };
    }

    Array3<std::vector<std::unique_ptr<IBodyProducer>>, Size> map;
    ChunkType type = ChunkType::NOT_GENERATED;
};

#endif //CHUNK_H
