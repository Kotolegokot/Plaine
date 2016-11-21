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

#ifndef IOBSTACLEPATTERN_H
#define IOBSTACLEPATTERN_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "IObstacle.h"
#include "util.h"

using namespace irr;

class IObstaclePattern
{
public:
    IObstaclePattern(int id) :
        m_id(id) {}
    virtual ~IObstaclePattern() = default;

    int id() const { return m_id; }
    virtual Vector3<int> size() const = 0;

    // create bodies and put them to the list
    // returns number of the bodies created
    virtual std::size_t produce(btDynamicsWorld &world,
                                IrrlichtDevice &device,
                                btVector3 position,
                                std::list<std::unique_ptr<IObstacle>> &list) const = 0;

private:
    int m_id;
};

#endif // IOBSTACLEPATTERN_H
