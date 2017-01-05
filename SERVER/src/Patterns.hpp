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

#include <memory>
#include <array>
#include <btBulletDynamicsCommon.h>
#include "IPattern.hpp"
#include "patterns/Cube.hpp"
#include "patterns/Crystal.hpp"
#include "patterns/Tunnel.hpp"
#include "patterns/Alley.hpp"


class Patterns {
    static constexpr std::size_t CRYSTALS = 4;
    static constexpr std::size_t CUBES = 2;

    static constexpr std::size_t ALL = CRYSTALS + CUBES + 2;
public:
    Patterns() = delete;

    static const std::array<std::shared_ptr<IPattern>, ALL> all;
    static const std::array<std::shared_ptr<IPattern>, CRYSTALS> crystals;
    static const std::array<std::shared_ptr<IPattern>, CUBES> cubes;
};
