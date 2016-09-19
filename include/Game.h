#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht/irrlicht.h>
#include "EventReceiver.h"
#include "util.h"
#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    bool initialized = false;
    bool pause = false;

    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *smgr = nullptr;
    gui::IGUIEnvironment *guienv = nullptr;
    io::IFileSystem *fs = nullptr;
    EventReceiver *evRec = nullptr;

    // GUI
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIStaticText *screenSizeText = nullptr;

    // Scene
    scene::ICameraSceneNode *camera = nullptr;
    scene::ILightSceneNode *light = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode2 = nullptr;
    SceneNodeAnimatorCameraPlayer *cameraAnimator = nullptr;

    void error(const core::stringw &str) const;
    bool initializeDevice();
    void initializeGUI();
    void initializeScene();

    // Constants
    const core::stringw ERR_NOT_INITIALIZED = "device has failed to initialize!";
};

#endif // GAME_H
