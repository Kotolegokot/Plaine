#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "ObstacleGenerator.h"
#include "PlaneProducer.h"
#include "Plane.h"
#include "Explosion.h"
#include "Chunk.h"
#include "Config.h"
#include "options.h"

using namespace irr;

#if FAR_CAMERA_DISTANCE
constexpr btScalar CAMERA_DISTANCE = 600;
#else
constexpr btScalar CAMERA_DISTANCE = 200;
#endif // FAR_CAMERA_DISTANCE

class World {
public:
    World(IrrlichtDevice &irrlichtDevice, const ConfigData &configuration,
          const ChunkDB &chunkDB);
    ~World();

    void render(video::SColor color);
    void stepSimulation(btScalar timeStep, int maxSubSteps = 1,
                        btScalar fixedTimeStep = btScalar(1.) / btScalar(60.));

    void generate();
    void updateAspectRatio();

    bool gameOver() const;
    std::size_t obstacles() const;

    Plane &plane();
private:
    std::unique_ptr<btBroadphaseInterface> m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_physicsWorld;

    std::unique_ptr<ObstacleGenerator> m_generator;
    std::unique_ptr<Plane> m_plane;
    std::unique_ptr<Explosion> m_explosion;

    IrrlichtDevice &m_irrlichtDevice;
    const ConfigData &m_configuration;
    const ChunkDB &m_chunkDB;

#if DEBUG_DRAWER_ENABLED
    std::unique_ptr<btIDebugDraw> debugDrawer;
#endif // DEBUG_DRAWER_ENABLED

    scene::ILightSceneNode &m_light;
    scene::ICameraSceneNode &m_camera;

    bool m_gameOver = false;
private:
    void updateCamera();
};

#endif // WORLD_H
