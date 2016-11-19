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
#include "Patterns.h"
#include "Chunk.h"

using namespace irr;

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;
using ChunkDB = std::array<Chunk<CHUNK_SIZE>, CHUNK_DB_SIZE>;

// this class is responsible for generating obstacles on the fly
class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice &device, btDynamicsWorld &world, btScalar farValue = 1500, btScalar buffer = CHUNK_LENGTH);
    void generate(const core::vector3df &playerPosition, const ChunkDB &chunkDB);
    u32 getCubeCount() const;

    void setFarValue(btScalar value);
    btScalar getFarValue() const;
    void setBuffer(btScalar buffer);
    btScalar getBuffer() const;

private:
    template <typename Num>
    struct Edges {
        Edges() = default;
        Edges(Num left, Num right, Num bottom, Num top, Num back, Num front) :
            left(left), right(right), bottom(bottom), top(top), back(back), front(front) {}

        template <typename Num2>
        Edges(const Edges<Num2> &other) :
            left(other.left), right(other.right), bottom(other.bottom),
            top(other.top), back(other.back), front(other.front) {}

        template <typename Num2>
        Edges<Num> &operator =(const Edges<Num2> &other)
        {
            left = other.left;
            right = other.right;
            bottom = other.bottom;
            top = other.top;
            back = other.back;
            front = other.front;
        }

        template <typename Num2>
        Edges(Edges<Num2> &&other) :
            left(other.left), right(other.right), bottom(other.bottom),
            top(other.top), back(other.back), front(other.front) {}

        template <typename Num2>
        Edges<Num> &operator =(Edges<Num2> &&other)
        {
            left = other.left;
            right = other.right;
            bottom = other.bottom;
            top = other.top;
            back = other.back;
            front = other.front;
        }

        template <typename Num2>
        bool operator ==(const Edges<Num2> &other) const
        {
            return left == other.left &&
                   right == other.right &&
                   bottom == other.bottom &&
                   top == other.top &&
                   back == other.back &&
                   front == other.front;
        }

        template <typename Num2>
        Edges<Num> operator /(Num2 n) const
        {
            return { left / n, right / n,
                     bottom / n, top / n,
                     back / n, front / n };
        }

        template <typename Num2>
        Edges<Num> operator /=(Num2 n)
        {
            left /= n;
            right /= n;
            bottom /= n;
            top /= n;
            back /= n;
            front /= n;

            return *this;
        }

        template <typename Num2>
        Edges<Num> operator %=(Num2 n)
        {
            left %= n;
            right %= n;
            bottom %= n;
            top %= n;
            back %= n;
            front %= n;

            return *this;
        }

        Num left = 0;
        Num right = 0;
        Num bottom = 0;
        Num top = 0;
        Num back = 0;
        Num front = 0;
    };

    Edges<btScalar> fieldOfView(const core::vector3df &playerPosition) const;

    // takes chunk coordinates and generate its appropriate part
    std::size_t insertCell(long x, long y, long z, const ChunkDB &chunkDB, Edges<long> cellEdges);

    static Point3<int> cellToChunk(const Point3<int> &cell);
    static Point3<int> relativeCellPos(const Point3<int> &cell, const Point3<int> &chunk);

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

    // edges denoting generated part of the world in cells
    Edges<long> generatedEdges;

    btDynamicsWorld &world; // physics world
};

#endif // OBSTACLEGENERATOR_H
