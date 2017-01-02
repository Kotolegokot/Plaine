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
#include "bodies/ConeFactory.hpp"

template <int Thickness, int Length>
class Crystal : public IPattern
{
public:
    Crystal(int id) : IPattern(id) {}

    Vector3<int> size() const override
    {
        return { Thickness, Length, Thickness };
    }

    std::vector<std::unique_ptr<IBodyFactory>> factories() const override
    {
        btVector3 position { Thickness * CELL_LENGTH * 0.5f,
                             Length * CELL_LENGTH * 0.5f,
                             Thickness * CELL_LENGTH * 0.5f };

        constexpr btScalar radius = (Thickness - 0.6f) * CELL_LENGTH * 0.5f;
        constexpr btScalar length = (Length - 0.2f) * CELL_LENGTH;

        std::vector<std::unique_ptr<IBodyFactory>> result;
        result.push_back(std::make_unique<ConeFactory>(radius, length * 0.5f));
        result.push_back(std::make_unique<ConeFactory>(radius, length * 0.5f));

        result[0]->rel_trans.setOrigin(position);
        result[1]->rel_trans.setOrigin(position);
        result[1]->rel_trans.setRotation(btQuaternion(0, 0, PI<btScalar>));

        return result;
    }
};
