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

#include "World.hpp"

World::World(IrrlichtDevice &irrlichtDevice, const ConfigData &configuration) :
    m_irrlichtDevice(irrlichtDevice),
    m_configuration(configuration),
    m_light(*m_irrlichtDevice.getSceneManager()->addLightSceneNode(0, { 0, 0, 0 }, DEFAULT_LIGHT_COLOR, 300)),
    m_camera(*m_irrlichtDevice.getSceneManager()->addCameraSceneNode(0))
{
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
}

World::~World()
{
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

void World::updateAspectRatio()
{
    m_camera.setAspectRatio(static_cast<f32>(m_configuration.resolution.Width) /
                            static_cast<f32>(m_configuration.resolution.Height));
}

bool World::gameOver() const
{
    return m_gameOver;
}

void World::updateCameraAndListener()
{
    core::vector3df upVector(0, 1, 0);
    //upVector.rotateXYBy(m_plane->getEulerRotationDeg().z());

    //m_camera.setPosition(m_plane->node().getPosition() + upVector * 0.3f * CAMERA_DISTANCE -
                         //core::vector3df(0, 0, CAMERA_DISTANCE));
    m_camera.setUpVector(upVector);

    m_camera.setTarget(m_camera.getPosition() + core::vector3df(0, 0, 1));
}
