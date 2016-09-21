#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht/irrlicht.h>
#include "EventReceiver.h"
#include "util.h"
#include "Config.h"
#include "SceneNodeAnimatorCameraPlayer.h"

using namespace irr;

class Game
{
public:
    Game();
    Game(struct ConfigData &data);
    ~Game();
    void run();
    void menu();

private:
    bool initialized = false;
    bool pause = false;

    bool fullscreen = false;
    bool customScreenSize = false;
    u32 colorDepth;
    core::stringw language;
    core::dimension2d<u32> windowSize;
    u32 colorDepth = 32;
    core::stringw language = L"English";

    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *sceneManager = nullptr;
    gui::IGUIEnvironment *guiEnvironment = nullptr;
    io::IFileSystem *fileSystem = nullptr;
    EventReceiver *eventReceiver = nullptr;

    // GUI
    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonToggleFullscreen = nullptr;
    gui::IGUIButton *buttonMenu = nullptr;
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIComboBox *resolutionComboBox = nullptr;
    gui::IGUIComboBox *colorDepthComboBox = nullptr;
    gui::IGUIComboBox *languageComboBox = nullptr;
    gui::IGUIStaticText *screenSizeText = nullptr;
    gui::IGUIStaticText *resolutionText = nullptr;
    gui::IGUIStaticText *colorDepthText = nullptr;
    gui::IGUIStaticText *languageText = nullptr;
    gui::IGUIStaticText *cameraPosText = nullptr;

    // Scene
    scene::ICameraSceneNode *camera = nullptr;
    scene::ILightSceneNode *light = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode2 = nullptr;

    void error(const core::stringw &str) const;
    bool initializeDevice();
    void initializeGUI();
    void initializeMenuGUI();
    void terminateMenuGUI();
    void initializeInGameGUI();
    void terminateInGameGUI();
    void initializeSettingsGUI();
    void terminateSettingsGUI();
    void initializeScene();
    void terminate();

    // Constants
    const core::stringw ERR_NOT_INITIALIZED = "device has failed to initialize!";
};

#endif // GAME_H
