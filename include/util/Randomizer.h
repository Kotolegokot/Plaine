/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

class Randomizer
{
public:
    Randomizer() = delete;

    template <typename RealType>
    static RealType getReal(RealType min = 0, RealType max = 1)
    {
        std::uniform_real_distribution<RealType> distribution(min, max);

        return distribution(engine);
    }

    template <typename IntType>
    static IntType getInteger(IntType min = 0, IntType max = std::numeric_limits<IntType>::max())
    {
        std::uniform_int_distribution<IntType> distribution(min, max);

        return distribution(engine);
    }

    static int getInt(int min = 0, int max = std::numeric_limits<int>::max())
    {
        return getInteger<int>(min, max);
    }

    static float getFloat(float min = 0, float max = 1)
    {
        return getReal<float>(min, max);
    }
private:
    static std::default_random_engine engine;
};

#endif // RANDOMIZER_H
