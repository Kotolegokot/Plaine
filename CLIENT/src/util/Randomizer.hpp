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

#include <random>

class Randomizer
{
public:
    Randomizer() = delete;

    template <typename RealType>
    static RealType get_real(RealType min = 0, RealType max = 1)
    {
        std::uniform_real_distribution<RealType> distribution(min, max);

        return distribution(engine);
    }

    template <typename IntType>
    static IntType get_integer(IntType min = 0, IntType max = std::numeric_limits<IntType>::max())
    {
        std::uniform_int_distribution<IntType> distribution(min, max);

        return distribution(engine);
    }

    static int get_int(int min = 0, int max = std::numeric_limits<int>::max())
    {
        return get_integer<int>(min, max);
    }

    static float get_float(float min = 0, float max = 1)
    {
        return get_real<float>(min, max);
    }
private:
    static std::default_random_engine engine;
};
