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

#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <list>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "options.h"
#include "MotionState.h"
#include "IObstacle.h"
#include "IBody.h"
#include "Randomizer.h"
#include "ObstaclePatternFactory.h"
#include "obstacles/Box.h"
#include "obstacles/Tetrahedron.h"
#include "obstacles/Icosahedron.h"
#include "obstacles/Icosphere2.h"
#include "obstacles/Cone.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"
#include "patterns/Valley.h"

using namespace irr;

// this class is responsible for generating obstacles on the fly
class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue = 1500, btScalar buffer = 300);
    ~ObstacleGenerator();
    void generate(const core::vector3df &playerPosition);
    u32 getCubeCount() const;

    void setFarValue(btScalar value);
    btScalar getFarValue() const;
    void setBuffer(btScalar buffer);
    btScalar getBuffer() const;

private:
    static constexpr btScalar STEP = 800;
    static constexpr btScalar CHUNK_SIZE = 1000;

    void stickToGrid(const core::vector3df &playerPosition,
                     std::size_t &edgeLeft, std::size_t &edgeRight,
                     std::size_t &edgeBottom, std::size_t &edgeTop,
                     std::size_t &edgeBack, std::size_t &edgeFront) const;
    unsigned long generateChunk(std::size_t x, std::size_t y, std::size_t z);
    void removeLeftBehind(btScalar playerZ);
    btScalar farValueWithBuffer() const;

    IrrlichtDevice &device;
    std::list<std::unique_ptr<IObstacle>> obstacles;

    u32 obstacleCount = 0;

    btScalar farValue = 0;
    // buffer is used to generate obstacles a bit farther than
    //      the camera's far value so that player sees them
    //      smoothly floating into the view range
    btScalar buffer = 0;

    std::size_t generatedEdgeFront = 1;
    std::size_t generatedEdgeLeft = 0;
    std::size_t generatedEdgeRight = 0;
    std::size_t generatedEdgeTop = 0;
    std::size_t generatedEdgeBottom = 0;

    btDynamicsWorld &world; // physics world

    ObstaclePatternFactory obstaclePatternFactory;
};

#endif // OBSTACLEGENERATOR_H
