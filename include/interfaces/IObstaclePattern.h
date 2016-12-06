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

#ifndef IOBSTACLEPATTERN_H
#define IOBSTACLEPATTERN_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IBodyProducer.h"
#include "util/other.h"

using namespace irr;

class IObstaclePattern
{
public:
    IObstaclePattern(int id) :
        m_id(id) {}
    virtual ~IObstaclePattern() = default;

    int id() const { return m_id; }
    virtual Vector3<int> size() const = 0;

    // returns body producers that are later used
    // in Chunk to create bodies
    virtual std::vector<std::unique_ptr<IBodyProducer>> producers() const = 0;

private:
    const int m_id;
};

#endif // IOBSTACLEPATTERN_H
