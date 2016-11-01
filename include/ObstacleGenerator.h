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
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "options.h"
#include "MotionState.h"
#include "IObstacle.h"
#include "IBody.h"
#include "Randomizer.h"
#include "obstacles/Box.h"
#include "obstacles/Tetrahedron.h"
#include "obstacles/Icosahedron.h"
#include "obstacles/Icosphere2.h"
#include "obstacles/Cone.h"
#include "patterns/Crystal.h"
#include "patterns/Tunnel.h"

// average distance between obstacles
constexpr btScalar STEP = 800;
constexpr btScalar START = 3000;

using namespace irr;

// this class is responsible for generating obstacles on the fly
//
// don't forget that this class is just a sample
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
    btScalar preciseEdge(btScalar edge) const;
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

    btScalar generatedEdgeZ = 0;
    btScalar generatedEdgeLeft = 0;
    btScalar generatedEdgeRight = 0;
    btScalar generatedEdgeTop = 0;
    btScalar generatedEdgeBottom = 0;

    // physics world
    btDynamicsWorld &world;
};

#endif // OBSTACLEGENERATOR_H
