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
#include <cstdint>
#include "Patterns.hpp"
#include "util/Randomizer.hpp"
#include "util/Array3.hpp"

template <int Size>
class Chunk {
private:
    // Point is a combination of a pattern and its position in a chunk
    using Point = std::pair<std::shared_ptr<IPattern>, Vector3i>;

    enum Type { CLOUD, RANDOM, NOT_GENERATED };
public:
    Chunk() = default;

    void generate()
    {
        std::vector<Point> points;

        switch (Randomizer::get_int(0, 2)) {
        case 0: { // cloud of crystals
            std::size_t n;
            do {
                points.clear();

                points.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int pattern_i = Randomizer::get_int(0, Patterns::crystals.size() - 1);

                    points.push_back(random_pos(Patterns::crystals[pattern_i]));
                }
            } while ((n = collisions(points)) < Size * Size / 8);
            points.resize(n);
            type = CLOUD;

            break;
        }

        case 1: { // cloud of cubes
            std::size_t n;
            do {
                points.clear();

                points.reserve(Size * Size / 4);
                for (std::size_t i = 0; i < Size * Size / 4; i++) {
                    const int pattern_i = Randomizer::get_int(0, Patterns::cubes.size() - 1);

                    points.push_back(random_pos(Patterns::cubes[pattern_i]));
                }
            } while ((n = collisions(points)) < Size * Size / 8);
            points.resize(n);
            type = CLOUD;

            break;
        }

        case 2: // absolutely random chunk
            do {
                points.clear();

                const std::size_t count = Randomizer::get_int(5, 10);
                points.reserve(count);
                for (std::size_t i = 0; i < count; i++) {
                    const int pattern_i = Randomizer::get_int(0, Patterns::all.size() - 1);

                    points.push_back(random_pos(Patterns::all[pattern_i]));
                }
            } while (collisions(points) != points.size());
            type = RANDOM;

            break;

        default:
            break;
        }

        generate_map(points);
    }

    // creates objects and returns number of bodies generated
    std::size_t produce_cell(btDynamicsWorld &world,
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
    std::size_t collisions(const std::vector<Point> &points)
    {
        std::size_t result = 0;
        /* 0     = empty
         * n > 0 = obstacle patterns
         */
        Array3<std::uintptr_t, Size> data;
        data.fill(0);

        for (const auto &point : points) {
            const auto &pattern = point.first;
            const Vector3i &pos = point.second;

            for (int x = 0; x < pattern->size().x; x++)
            for (int y = 0; y < pattern->size().y; y++)
            for (int z = 0; z < pattern->size().z; z++) {
                std::uintptr_t &current_cell = data.at(pos + Vector3i(x, y, z));

                if (current_cell != 0)
                    return result;
                else
                    data.at(pos + Vector3i(x, y, z)) = reinterpret_cast<std::uintptr_t>(pattern.get());
            }

            result++;
        }

        return result;
    }

    void generate_map(const std::vector<Point> &points)
    {
        // clear the map
        std::for_each(map.begin(), map.end(), [](auto &v) { v.clear(); });

        for (const auto &point : points) {
            const auto pattern = point.first;
            const Vector3i &pos = point.second;

            for (auto &factory : pattern->factories()) {
                // factory position relative to pattern
                btVector3 &origin = factory->rel_trans.getOrigin();
                origin += pos * CELL_LENGTH;

                // factory cell relative to chunk
                const Vector3i cell = static_cast<Vector3i>((Vector3i(origin) / CELL_LENGTH));

                // factory position relative to cell
                origin -= cell * CELL_LENGTH;

                map[cell].push_back(std::move(factory));
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

    static Point random_pos(std::shared_ptr<IPattern> pattern)
    {
        Vector3i pos { Randomizer::get_int(0, Size - pattern->size().x),
                       Randomizer::get_int(0, Size - pattern->size().y),
                       Randomizer::get_int(0, Size - pattern->size().z) };
        return std::make_pair(pattern, pos);
    }

    Array3<std::vector<std::unique_ptr<IBodyFactory>>, Size> map;
    Type type = NOT_GENERATED;
};
