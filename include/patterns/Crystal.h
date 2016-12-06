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

#ifndef CRYSTAL_H
#define CRYSTAL_H

#include <memory>
#include "interfaces/IBodyProducer.h"
#include "interfaces/IObstaclePattern.h"
#include "bodies/ConeProducer.h"
#include "util/other.h"

template <int Thickness, int Length>
class Crystal : public IObstaclePattern
{
public:
    Crystal(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { Thickness, Length, Thickness };
    }

    std::vector<std::unique_ptr<IBodyProducer>>
        producers() const override
    {
        btVector3 position { Thickness * CELL_LENGTH * 0.5f,
                             Length * CELL_LENGTH * 0.5f,
                             Thickness * CELL_LENGTH * 0.5f };

        constexpr btScalar radius = (Thickness - 0.6f) * CELL_LENGTH * 0.5f;
        constexpr btScalar length = (Length - 0.2f) * CELL_LENGTH;

        std::vector<std::unique_ptr<IBodyProducer>> result;
        result.push_back(std::make_unique<ConeProducer>(radius, length * 0.5f));
        result.push_back(std::make_unique<ConeProducer>(radius, length * 0.5f));

        result[0]->relativeTransform.setOrigin(position);
        result[1]->relativeTransform.setOrigin(position);
        result[1]->relativeTransform.setRotation(btQuaternion(0, 0, PI<btScalar>));

        return result;
    }
};

#endif // CRYSTAL_H
