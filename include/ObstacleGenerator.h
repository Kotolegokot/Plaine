#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include <deque>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "MotionState.h"

using namespace irr;

class ObstacleGenerator
{
public:
    ObstacleGenerator(IrrlichtDevice *device, btDynamicsWorld *world, f32 farValue = 1500, f32 buffer = 300);
    ~ObstacleGenerator();
    void generate(const core::vector3df &playerPosition);
    u32 getCubeCount() const;

    void setFarValue(f32 value);
    f32 getFarValue() const;
    void setBuffer(f32 buffer);
    f32 getBuffer() const;

private:
    f32 preciseEdge(f32 edge) const;
    void removeLeftBehind(f32 playerZ);
    f32 farValueWithBuffer() const;
    btRigidBody *createCube(const core::vector3df &position);

    IrrlichtDevice *device = nullptr;
    std::deque<btRigidBody *> cubes;

    u32 cubeCount = 0;

    f32 farValue = 0;
    f32 buffer = 0;

    f32 generatedEdgeZ = 0;
    f32 generatedEdgeLeft = 0;
    f32 generatedEdgeRight = 0;
    f32 generatedEdgeTop = 0;
    f32 generatedEdgeBottom = 0;

    const f32 STEP = 400;
    const f32 CUBE_SIDE = 250;

    // Bullet
    btCollisionShape *cubeShape = nullptr;
    btVector3 cubeInertia = btVector3(0, 0, 0);
    btRigidBody::btRigidBodyConstructionInfo *cubeConstructionInfo = nullptr;

    btDynamicsWorld *world = nullptr;
};

#endif // OBSTACLEGENERATOR_H
