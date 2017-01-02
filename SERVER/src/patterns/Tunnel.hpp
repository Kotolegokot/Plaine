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
#include "IBodyFactory.hpp"
#include "IPattern.hpp"
#include "bodies/BoxFactory.hpp"

class Tunnel : public IPattern
{
public:
    Tunnel(int id) : IPattern(id) {}

    Vector3<int> size() const override
    {
        return { 1, 1, 2 };
    }

    std::vector<std::unique_ptr<IBodyFactory>> factories() const override
    {
        btVector3 position { CELL_LENGTH * 0.5f, CELL_LENGTH * 0.5f, CELL_LENGTH };

        constexpr btScalar radius = CELL_LENGTH * 0.4f;
        constexpr btScalar length = CELL_LENGTH * 1.8f;

        std::vector<std::unique_ptr<IBodyFactory>> result;

        result.push_back(std::make_unique<BoxFactory>(btVector3(radius / 10, radius, length / 2)));
        result.back()->rel_trans.setOrigin(position + btVector3(radius, 0, 0));

        result.push_back(std::make_unique<BoxFactory>(btVector3(radius / 10, radius, length / 2)));
        result.back()->rel_trans.setOrigin(position + btVector3(-radius, 0, 0));

        result.push_back(std::make_unique<BoxFactory>(btVector3(radius, radius / 10, length / 2)));
        result.back()->rel_trans.setOrigin(position + btVector3(0, radius, 0));

        result.push_back(std::make_unique<BoxFactory>(btVector3(radius, radius / 10, length / 2)));
        result.back()->rel_trans.setOrigin(position + btVector3(0, -radius, 0));

        return result;
    }
};
