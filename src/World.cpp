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

#include "World.h"

World::World(IrrlichtDevice &irrlichtDevice, AudioDevice &audioDevice, const ConfigData &configuration,
             const ChunkDB &chunkDB) :
    m_broadphase(std::make_unique<btDbvtBroadphase>()),
    m_collisionConfiguration(std::make_unique<btDefaultCollisionConfiguration>()),
    m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get())),
    m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    m_irrlichtDevice(irrlichtDevice),
    m_audioDevice(audioDevice),
    m_configuration(configuration),
    m_chunkDB(chunkDB),
    m_light(*m_irrlichtDevice.getSceneManager()->addLightSceneNode(0, { 0, 0, 0 }, DEFAULT_LIGHT_COLOR, 300)),
    m_camera(*m_irrlichtDevice.getSceneManager()->addCameraSceneNode(0))
{
    // physics
    {
        btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher.get());

        m_physicsWorld = std::make_unique<btDiscreteDynamicsWorld>
                (m_dispatcher.get(), m_broadphase.get(),
                 m_solver.get(), m_collisionConfiguration.get());
        m_physicsWorld->setGravity({ 0, 0, 0 });

        m_plane = PlaneProducer().producePlane(*m_physicsWorld, m_irrlichtDevice);
        m_explosion = std::make_unique<Explosion>(*m_physicsWorld, m_irrlichtDevice,
                                                  m_plane->getPosition(), 1000);

        /*gContactProcessedCallback = [](btManifoldPoint &cp, void *obj0p, void *obj1p) -> bool
            {
                auto obj0 = static_cast<btCollisionObject *>(obj0p);
                auto obj1 = static_cast<btCollisionObject *>(obj1p);

                // if one of the objects is the place
                if (obj0->getUserIndex() == 1 || obj1->getUserIndex() == 1) {
                    Log::debug("plane collision occured");
                    Log::debug("collision impulse = ", cp.getAppliedImpulse());

                    // obj0 must always be the plane
                    if (obj1->getUserIndex() == 1)
                        std::swap(obj0, obj1);


                    Plane &plane = *static_cast<Plane *>(obj0->getUserPointer());

                    auto sound = m_audioDevice.createSound("media/sounds/collision.mp3");
                    sound->setPosition(plane.getPosition());
                    sound->setDirection(plane.getRotation().getAxis());
                    sound->setVelocity(plane.getLinearVelocity());
                    sound->play();

                    if (cp.getAppliedImpulse() > 400)
                        plane.explode();
                    else if (!plane.exploded())
                        plane.addScore(-cp.getAppliedImpulse());
                }

                return true;
            };*/
    }

    // graphics
    {
#if FOG_ENABLED
        m_irrlichtDevice.getVideoDriver()->setFog(DEFAULT_COLOR, video::EFT_FOG_LINEAR,
                                                  configuration.renderDistance - 300,
                                                  configuration.renderDistance, .003f, true, false);
#endif // FOG_ENABLED

        m_camera.setFarValue(m_configuration.renderDistance);
        updateCameraAndListener();

        m_light.setLightType(video::ELT_DIRECTIONAL);
        {
            video::SLight lightData = m_light.getLightData();
            lightData.DiffuseColor = DEFAULT_LIGHT_COLOR;
            lightData.AmbientColor = DEFAULT_LIGHT_COLOR;
            m_light.setLightData(lightData);
        }
    }

    // other stuff
    {
        m_generator = std::make_unique<ObstacleGenerator>
                (*m_physicsWorld, m_irrlichtDevice, m_camera.getFarValue(), 300);
    }

#if DEBUG_DRAWER_ENABLED
    debugDrawer = std::make_unique<DebugDrawer>(m_irrlichtDevice);
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_physicsWorld->setDebugDrawer(debugDrawer.get());
#endif // DEBUG_DRAWER_ENABLED
}

World::~World()
{
    // obstacle generator must be deleted before clearing scene
    m_generator.reset();
    m_plane.reset();
    m_explosion.reset();
    m_physicsWorld.reset();
    m_irrlichtDevice.getSceneManager()->clear();
}

void World::render(video::SColor color)
{
#if FOG_ENABLED && IRIDESCENT_FOG
    m_irrlichtDevice.getVideoDriver()->setFog(color, video::EFT_FOG_LINEAR,
                                              m_configuration.renderDistance - 300,
                                              m_configuration.renderDistance, 0.01f, true, true);
#endif // FOG_ENABLED && IRIDESCENT_FOG

#if IRIDESCENT_LIGHT
    video::SLight lightData = m_light.getLightData();
    lightData.DiffuseColor = color;
    lightData.AmbientColor = color;
    m_light.setLightData(lightData);
#endif // IRIDESCENT_LIGHT

#if DEBUG_DRAWER_ENABLED
    m_physicsWorld->debugDrawWorld();
#endif // DEBUG_DRAWER_ENABLED

    if (!m_gameOver)
        updateCameraAndListener(); // update camera position, target, and rotation
    m_irrlichtDevice.getSceneManager()->drawAll();
}

void World::stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
    m_explosion->setPosition(m_plane->getPosition());
    m_gameOver = m_plane->exploded();

    m_physicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);

    Log::debug("simulation step = ", timeStep, "ms");
}

void World::generate()
{
    m_generator->generate(m_plane->getPosition(), m_chunkDB);
}

void World::updateAspectRatio()
{
    m_camera.setAspectRatio(static_cast<f32>(m_configuration.resolution.Width) /
                            static_cast<f32>(m_configuration.resolution.Height));
}

bool World::gameOver() const
{
    return m_gameOver;
}

std::size_t World::obstacles() const
{
    return m_generator->obstacles();
}

Plane &World::plane()
{
    return *m_plane;
}

void World::updateCameraAndListener()
{
    core::vector3df upVector(0, 1, 0);
    upVector.rotateXYBy(m_plane->getEulerRotationDeg().z());

    m_camera.setPosition(m_plane->node().getPosition() + upVector * 0.3f * CAMERA_DISTANCE -
                         core::vector3df(0, 0, CAMERA_DISTANCE));
    m_camera.setUpVector(upVector);

    m_camera.setTarget(m_camera.getPosition() + core::vector3df(0, 0, 1));

    AudioListener::setPosition(m_plane->getPosition());
    AudioListener::setUpVector(upVector);
    AudioListener::setDirection(m_plane->getPosition() + btVector3(0, 0, 1));
}
