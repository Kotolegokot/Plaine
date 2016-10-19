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

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <CGUITTFont.h>
#include <ITimer.h>
#include "EventReceiver.h"
#include "util.h"
#include "Config.h"
#include "GUI.h"
#include "ObstacleGenerator.h"
#include "Plane.h"
#include "PlaneControl.h"
#include "DebugDrawer.h"
#include "options.h"
#include "Explosion.h"

using namespace irr;

// this class describes the whole game from main menu
// to plane controlling
class Game
{
public:
    Game(const struct ConfigData &data = ConfigData());
    ~Game();
    void run();
    void menu();

private:
    bool initialized = false;
    bool pause = false;

    ConfigData configuration;
    GUI *gui = nullptr;
    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *sceneManager = nullptr;
    gui::IGUIEnvironment *guiEnvironment = nullptr;
    io::IFileSystem *fileSystem = nullptr;
    EventReceiver *eventReceiver = nullptr;
    gui::IGUISkin *skin = nullptr;
    ITimer *timer = nullptr;

    // Bullet
    btBroadphaseInterface *broadphase = nullptr;
    btDefaultCollisionConfiguration *collisionConfiguration = nullptr;
    btCollisionDispatcher *dispatcher = nullptr;
    btSequentialImpulseConstraintSolver *solver = nullptr;
    btDiscreteDynamicsWorld *dynamicsWorld = nullptr;

    #if DEBUG_DRAWER_ENABLED
        btIDebugDraw *debugDrawer = nullptr;
    #endif // DEBUG_DRAWER_ENABLED

    // Scene
    scene::ILightSceneNode *light = nullptr;
    scene::ICameraSceneNode *camera = nullptr;
    ObstacleGenerator *obstacleGenerator = nullptr;
    Plane *plane = nullptr;
    PlaneControl *planeControl = nullptr;
    Explosion *explosion = nullptr;

    void error(const core::stringw &str) const;
    bool initializeDevice();
    void initializeGUI();
    void initializeBullet();
    void initializeScene();
    void terminateScene();
    void terminateDevice();

    void updateHUD();
    void updateCamera();
    // returns false if need to quit pause menu
    bool handlePause(video::SColor &color);
};

#endif // GAME_H
