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

#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include "Patterns.hpp"
#include "util/Randomizer.hpp"
#include "util/Array3.hpp"

template <int Size>
class Chunk {
private:
    struct PatternPos {
        PatternPos() = default;
        PatternPos(std::shared_ptr<IPattern> pattern, Vector3i pos) :
            pattern(pattern), pos(pos) {}
        PatternPos(const PatternPos &) = default;
        PatternPos &operator =(const PatternPos &) = default;
        PatternPos(PatternPos &&) noexcept = default;
        PatternPos &operator =(PatternPos &&) noexcept = default;

        std::shared_ptr<IPattern> pattern;
        Vector3i pos;
    };

    enum Type { CLOUD, RANDOM, NOT_GENERATED };
public:
    Chunk() = default;

    void generate()
    {
        std::vector<PatternPos> poss;

        switch (Randomizer::get_int(0, 2)) {
        case 0: { // cloud of crystals
            std::size_t n;
            do {
                poss.clear();

                poss.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int patternIndex = Randomizer::get_int(0, Patterns::crystals.size() - 1);

                    poss.push_back(random_pos(Patterns::crystals[patternIndex]));
                }
            } while ((n = collisions(poss)) < Size * Size / 8);
            poss.resize(n);
            type = CLOUD;

            break;
        }

        case 1: { // cloud of cubes
            std::size_t n;
            do {
                poss.clear();

                poss.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int patternIndex = Randomizer::get_int(0, Patterns::cubes.size() - 1);

                    poss.push_back(random_pos(Patterns::cubes[patternIndex]));
                }
            } while ((n = collisions(poss)) < Size * Size / 8);
            poss.resize(n);
            type = CLOUD;

            break;
        }

        case 2: // absolutely random chunk
            do {
                poss.clear();

                const std::size_t count = Randomizer::get_int(5, 10);
                poss.reserve(count);
                for (std::size_t i = 0; i < count; i++) {
                    const int patternIndex = Randomizer::get_int(0, Patterns::all.size() - 1);

                    poss.push_back(random_pos(Patterns::all[patternIndex]));
                }
            } while (collisions(poss) != poss.size());
            type = RANDOM;

            break;

        default:
            break;
        }

        generate_map(poss);
    }

    // creates objects and returns number of bodies generated
    std::size_t produceCell(btDynamicsWorld &world,
                            btVector3 chunk,
                            Vector3i cell,
                            std::list<std::unique_ptr<btRigidBody>> &list) const
    {
        if (type == NOT_GENERATED)
            return 0;

        std::size_t generated = 0;

        for (const auto &factory : map[cell]) {
            auto body = factory->produce(chunk + cell * CELL_LENGTH);
            world.addRigidBody(body.get());
            list.push_back(std::move(body));

            generated++;
        }

        return generated;
    }

private:
    // returns how many obstacles it managed to fit into the chunk
    std::size_t collisions(const std::vector<PatternPos> &poss)
    {
        std::size_t result = 0;
        /* 0     = empty
         * n > 0 = obstacle patterns
         */
        Array3<int, Size> data;
        data.fill(0);

        for (const auto &pos : poss) {
            const auto &pattern = pos.pattern;
            const Vector3i &ppos = pos.pos;

            for (int x = 0; x < pattern->size().x; x++)
            for (int y = 0; y < pattern->size().y; y++)
            for (int z = 0; z < pattern->size().z; z++) {
                int &currentCell = data.at(ppos + Vector3i(x, y, z));

                if (currentCell != 0)
                    return result;
                else
                    data.at(ppos + Vector3i(x, y, z)) = pattern->id() + 1;
            }

            result++;
        }

        return result;
    }

    void generate_map(const std::vector<PatternPos> &poss)
    {
        // clear the map
        std::for_each(map.begin(), map.end(), [](auto &vector) { vector.clear(); });

        for (const auto &pos : poss) {
            const auto &pattern = *pos.pattern;
            const Vector3i &ppos = pos.pos;

            for (auto &producer : pattern.producers()) {
                // producer pos relative to pattern
                btVector3 &origin = producer->rel_trans.getOrigin();
                origin += ppos * CELL_LENGTH;

                // producer cell relative to chunk
                const Vector3i cell = (Vector3i(origin) / CELL_LENGTH);

                // producer pos relative to cell
                origin -= cell * CELL_LENGTH;

                map[cell].push_back(std::move(producer));
            }
        }
    }

    static Vector3i cell_pos(const btVector3 &pos)
    {
        return Vector3i(pos) / CELL_LENGTH;
    }

    static btVector3 cell_rel_pos(const btVector3 &pos, const Vector3i &cell)
    {
        return pos - cell * CELL_LENGTH;
    }

    static PatternPos random_pos(std::shared_ptr<IPattern> pattern)
    {
        return { pattern,
                 { Randomizer::get_int(0, Size - pattern->size().x),
                   Randomizer::get_int(0, Size - pattern->size().y),
                   Randomizer::get_int(-1, Size - pattern->size().z) } };
    }

    Array3<std::vector<std::unique_ptr<IBodyFactory>>, Size> map;
    Type type = NOT_GENERATED;
};
