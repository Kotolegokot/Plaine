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

#ifndef VALLEY_H
#define VALLEY_H

#include <memory>
#include "interfaces/IBodyProducer.h"
#include "interfaces/IObstaclePattern.h"
#include "bodies/IcosahedronProducer.h"
#include "util.h"

template <int Length>
class Alley : public IObstaclePattern
{
public:
    Alley(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { 3, 3, Length * 2 };
    }

    std::vector<std::unique_ptr<IBodyProducer>>
        producers() const override
    {
        std::vector<std::unique_ptr<IBodyProducer>> result;

        btVector3 position { 1.5f * CELL_LENGTH, 1.5f * CELL_LENGTH, 0 };

        static constexpr btScalar cos45 = 0.70710678118;
        static constexpr btScalar edge = 150;

        for (std::size_t i = 0; i < Length; i++) {
            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativeTransform.setOrigin(position + (i % 2 == 0 ?
                          btVector3(-CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                          btVector3(-CELL_LENGTH * cos45, CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativeTransform.setOrigin(position + (i % 2 == 0 ?
                          btVector3(CELL_LENGTH, 0, CELL_LENGTH * i * 2) :
                          btVector3(CELL_LENGTH * cos45, -CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativeTransform.setOrigin(position + (i % 2 == 0 ?
                          btVector3(0, -CELL_LENGTH, CELL_LENGTH * i * 2) :
                          btVector3(-CELL_LENGTH * cos45, -CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));

            result.push_back(std::make_unique<IcosahedronProducer>(edge));
            result.back()->relativeTransform.setOrigin(position + (i % 2 == 0 ?
                          btVector3(0, CELL_LENGTH, CELL_LENGTH * i * 2) :
                          btVector3(CELL_LENGTH * cos45, CELL_LENGTH * cos45,
                                    CELL_LENGTH * i * 2)));
        }

        return result;
    }

};
#endif // VALLEY_H
