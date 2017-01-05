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

#include <vector>

#include "IBodyFactory.hpp"
#include "util/Vector3.hpp"

struct IPattern
{
    virtual ~IPattern() = default;

    virtual Vector3i size() const = 0;

    // return body factories that can be
    // then used in Chunk
    virtual std::vector<std::unique_ptr<IBodyFactory>> factories() const = 0;
};
