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

#include "Patterns.hpp"

constexpr std::size_t Patterns::CRYSTALS;
constexpr std::size_t Patterns::CUBES;
constexpr std::size_t Patterns::ALL;

const std::array<std::shared_ptr<IPattern>, Patterns::CRYSTALS> Patterns::crystals = {
    {
        std::make_shared<Crystal<1, 2>>(),
        std::make_shared<Crystal<1, 4>>(),
        std::make_shared<Crystal<2, 4>>(),
        std::make_shared<Crystal<3, 8>>()
    }
};

const std::array<std::shared_ptr<IPattern>, Patterns::CUBES> Patterns::cubes = {
    {
        std::make_shared<Cube<1>>(),
        std::make_shared<Cube<2>>()
    }
};

const std::array<std::shared_ptr<IPattern>, Patterns::ALL> Patterns::all = {
    {
        Patterns::crystals[0],
        Patterns::crystals[1],
        Patterns::crystals[2],
        Patterns::crystals[3],
        Patterns::cubes[0],
        Patterns::cubes[1],
        std::make_shared<Tunnel>(),
        std::make_shared<Alley<5>>()
    }
};

