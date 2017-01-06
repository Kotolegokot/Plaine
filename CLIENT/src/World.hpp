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

#pragma once

#include <iostream>
#include <memory>
#include <irrlicht.h>
#include "Config.hpp"
#include "Audio.hpp"
#include "Scoreboard.hpp"
#include "util/options.hpp"

using namespace irr;

#if FAR_CAMERA_DISTANCE
constexpr btScalar CAMERA_DISTANCE = 600;
#else
constexpr btScalar CAMERA_DISTANCE = 200;
#endif // FAR_CAMERA_DISTANCE

constexpr btScalar EXPLOSION_THRESHOLD = 400.0f;

class World {
    IrrlichtDevice &m_irrlichtDevice;
    const ConfigData &m_configuration;

    scene::ILightSceneNode &m_light;
    scene::ICameraSceneNode &m_camera;

    bool m_gameOver = false;
    void updateCameraAndListener();
public:
    World(IrrlichtDevice &irrlichtDevice, const ConfigData &configuration);
    ~World();

    void render(video::SColor color);
    void updateAspectRatio();
    bool gameOver() const;
};
