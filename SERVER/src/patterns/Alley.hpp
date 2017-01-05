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
#include "bodies/IcosahedronFactory.hpp"

template <int Length>
class Alley : public IPattern
{
public:
    Vector3<int> size() const override
    {
        return { 3, 3, Length * 2 };
    }

    std::vector<std::unique_ptr<IBodyFactory>> factories() const override
    {
        std::vector<std::unique_ptr<IBodyFactory>> result;

        btVector3 position { 1.5f * CELL_LENGTH, 1.5f * CELL_LENGTH, 0 };

        static constexpr btScalar cos45 = 0.70710678118;
        static constexpr btScalar edge = 150;

        for (std::size_t i = 0; i < Length; i++) {
            result.push_back(std::make_unique<IcosahedronFactory>(edge));
            result.back()->rel_trans.setOrigin(position + (i % 2 == 0 ?
                          btVector3(-CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                          btVector3(-CELL_LENGTH * cos45, CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronFactory>(edge));
            result.back()->rel_trans.setOrigin(position + (i % 2 == 0 ?
                          btVector3(CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                          btVector3(CELL_LENGTH * cos45, -CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronFactory>(edge));
            result.back()->rel_trans.setOrigin(position + (i % 2 == 0 ?
                          btVector3(0, -CELL_LENGTH, CELL_LENGTH * i * 2) :
                          btVector3(-CELL_LENGTH * cos45, -CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronFactory>(edge));
            result.back()->rel_trans.setOrigin(position + (i % 2 == 0 ?
                          btVector3(0, CELL_LENGTH, CELL_LENGTH * i * 2) :
                          btVector3(CELL_LENGTH * cos45, CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));
        }

        return result;
    }

};
