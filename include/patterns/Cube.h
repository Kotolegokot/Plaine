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

#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include "interfaces/IBodyProducer.h"
#include "interfaces/IObstaclePattern.h"
#include "bodies/BoxProducer.h"
#include "util/other.h"

using namespace irr;

template <int Size>
class Cube : public IObstaclePattern
{
public:
    Cube(int id) :
        IObstaclePattern(id) {}

    Vector3<int> size() const override
    {
        return { Size, Size, Size };
    }

    std::vector<std::unique_ptr<IBodyProducer>>
        producers() const override
    {
        btVector3 position { Size * CELL_LENGTH * 0.5f,
                             Size * CELL_LENGTH * 0.5f,
                             Size * CELL_LENGTH * 0.5f };

        std::vector<std::unique_ptr<IBodyProducer>> result;
        result.push_back(std::make_unique<BoxProducer>(btVector3(1, 1, 1) *
                                                       Size * CELL_LENGTH * 0.49));
        result.back()->relativeTransform.setOrigin(position);
        result.back()->relativeTransform.setRotation(btQuaternion(0, 0, PI<btScalar> * 0.5f));

        return result;
    }
};

#endif // CUBE_H
