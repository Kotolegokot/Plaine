#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <CGUITTFont.h>
#include "EventReceiver.h"
#include "util.h"
#include "Config.h"
#include "GUI.h"
#include "SceneNodeAnimatorCameraPlayer.h"
#include "ObstacleGenerator.h"

using namespace irr;

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

    // Bullet
    btBroadphaseInterface *broadphase = nullptr;
    btDefaultCollisionConfiguration *collisionConfiguration = nullptr;
    btCollisionDispatcher *dispatcher = nullptr;
    btSequentialImpulseConstraintSolver *solver = nullptr;
    btDiscreteDynamicsWorld *dynamicsWorld = nullptr;

    // Scene
    scene::ICameraSceneNode *camera = nullptr;
    scene::ILightSceneNode *light = nullptr;
    ObstacleGenerator *obstacleGenerator = nullptr;

    void error(const core::stringw &str) const;
    bool initializeDevice();
    void initializeGUI();
    void initializeScene();
    void terminateScene();
    void terminateDevice();

    // Constants
    const core::stringw ERR_NOT_INITIALIZED = "device has failed to initialize!";
};

#endif // GAME_H
