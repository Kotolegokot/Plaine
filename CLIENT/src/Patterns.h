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

#ifndef PATTERNS_H
#define PATTERNS_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "interfaces/IObstaclePattern.h"
#include "patterns/Cube.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Alley.h"


class Patterns {
private:
    static constexpr std::size_t CRYSTALS = 4;
    static constexpr std::size_t CUBES = 2;

    static constexpr std::size_t ALL = CRYSTALS + CUBES + 2;
public:
    Patterns() = delete;

    static const std::array<std::shared_ptr<IObstaclePattern>, ALL> all;
    static const std::array<std::shared_ptr<IObstaclePattern>, CRYSTALS> crystals;
    static const std::array<std::shared_ptr<IObstaclePattern>, CUBES> cubes;
};

#endif // PATTERNS_H
