#include "Game.h"

using namespace irr;

Game::Game()
{
    initializeGUI();
    initializeScene();
    initialized = true;
}

Game::Game(const struct ConfigData &data)
{
    configuration = data;
    if (!initializeDevice())
        return;
    initializeGUI();
    initialized = true;
}

Game::~Game()
{
    device->drop();
}

void Game::initializeGUI()
{
    gui::IGUISkin *skin = guiEnvironment->getSkin();
    gui::IGUIFont *font = gui::CGUITTFont::createTTFont(driver, fileSystem, io::path("media/fonts/font.ttf"), 13, true, true);
    if (font)
        skin->setFont(font);
}

bool Game::initializeDevice()
{
    if (configuration.fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        configuration.resolution = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }
    device = createDevice(video::EDT_OPENGL, configuration.resolution, configuration.colordepth, configuration.fullscreen);
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
    device->setWindowCaption(L"PlaneTest");
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();
    guiEnvironment = device->getGUIEnvironment();
    fileSystem = device->getFileSystem();
    eventReceiver = new EventReceiver();
    device->setEventReceiver(eventReceiver);
    device->setResizable(customResolution);
    return true;
}

void Game::initializeMenuGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    buttonWidth = configuration.resolution.Width / 6;
    if (buttonWidth < 180)
        buttonWidth = 180;
    buttonHeight = buttonWidth * 1/8;
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - buttonWidth - 20, screenSize.Height - 3*buttonHeight - 30, screenSize.Width - 20, screenSize.Height - 2*buttonHeight - 30), 0, ID_BUTTON_START, L"Start", L"Start game");
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - buttonWidth - 20, screenSize.Height - 2*buttonHeight - 20, screenSize.Width - 20, screenSize.Height - buttonHeight - 20), 0, ID_BUTTON_SETTINGS, L"Settings", L"Game settings");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - buttonWidth - 20, screenSize.Height - buttonHeight - 10, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
}

void Game::terminateMenuGUI()
{
    buttonStart->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    screenSizeText->remove();
}

void Game::initializeInGameGUI()
{
    buttonWidth = configuration.resolution.Width / 6;
    if (buttonWidth < 180)
        buttonWidth = 180;
    buttonHeight = buttonWidth * 1/8;
    cameraPosText = guiEnvironment->addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20), 0, ID_BUTTON_MENU, L"Menu", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
}

void Game::terminateInGameGUI()
{
    buttonMenu->remove();
    buttonQuit->remove();
    cameraPosText->remove();
    screenSizeText->remove();
}

void Game::initializeSettingsGUI()
{
    buttonWidth = configuration.resolution.Width / 6;
    if (buttonWidth < 180)
        buttonWidth = 180;
    buttonHeight = buttonWidth * 1/8;
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    resolutionText = guiEnvironment->addStaticText(L"Resolution:", core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 6*buttonHeight - 55, configuration.resolution.Width - 135, configuration.resolution.Height - 5*buttonHeight - 55), false);
    resolutionComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 6*buttonHeight - 60, configuration.resolution.Width - 20, configuration.resolution.Height - 5*buttonHeight - 60), 0, ID_RESOLUTION_COMBOBOX);
    if (configuration.fullscreen)
    {
        resolutionComboBox->setEnabled(false);
        resolutionComboBox->addItem(L"Fullscreen", 0);
        resolutionComboBox->setSelected(0);
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30), 0, ID_BUTTON_TOGGLE_FULLSCREEN, L"Windowed", L"To Windewed Mode");

    }
    else {
        resolutionComboBox->addItem(L"640x480", 0);
        resolutionComboBox->addItem(L"1240x720", 1);
        resolutionComboBox->addItem(L"Custom Resolution", 2);
        if (customResolution)
            resolutionComboBox->setSelected(2);
        else if (configuration.resolution == core::dimension2d<u32>(640, 480))
            resolutionComboBox->setSelected(0);
        else if (configuration.resolution == core::dimension2d<u32>(1240, 720))
            resolutionComboBox->setSelected(1);
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30), 0, ID_BUTTON_TOGGLE_FULLSCREEN, L"Fullscreen", L"To Fullscreen Mode");

    }
    colorDepthText = guiEnvironment->addStaticText(L"Color Depth:", core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 5*buttonHeight - 45, configuration.resolution.Width - 130, configuration.resolution.Height - 4*buttonHeight - 45), false);
    colorDepthComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 5*buttonHeight - 50, configuration.resolution.Width - 20, configuration.resolution.Height - 4*buttonHeight - 50), 0, ID_COLORDEPTH_COMBOBOX);
    colorDepthComboBox->addItem(L"8", 0);
    colorDepthComboBox->addItem(L"16", 1);
    colorDepthComboBox->addItem(L"32", 2);
    if (configuration.colordepth == 8)
        colorDepthComboBox->setSelected(0);
    else if (configuration.colordepth == 16)
        colorDepthComboBox->setSelected(1);
    else if (configuration.colordepth == 32)
        colorDepthComboBox->setSelected(2);
    languageText = guiEnvironment->addStaticText(L"Language:", core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 4*buttonHeight - 35, configuration.resolution.Width - 130, configuration.resolution.Height - 3*buttonHeight - 35), false);
    languageComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 4*buttonHeight - 40, configuration.resolution.Width - 20, configuration.resolution.Height - 3*buttonHeight - 40), 0, ID_LANGUAGE_COMBOBOX);
    languageComboBox->addItem(L"English", 0);
    if (configuration.language == L"English")
        languageComboBox->setSelected(0);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20), 0, ID_BUTTON_MENU, L"Back", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
}

void Game::terminateSettingsGUI()
{
    buttonMenu->remove();
    buttonQuit->remove();
    buttonToggleFullscreen->remove();
    screenSizeText->remove();
    resolutionText->remove();
    resolutionComboBox->remove();
    colorDepthText->remove();
    colorDepthComboBox->remove();
    languageText->remove();
    languageComboBox->remove();
}

void Game::initializeScene()
{
    camera = sceneManager->addCameraSceneNode();
    scene::ISceneNodeAnimator *cameraAnimator = new SceneNodeAnimatorCameraPlayer(5.f, 5.f);
    camera->addAnimator(cameraAnimator);
    cameraAnimator->drop();

    light = sceneManager->addLightSceneNode(0, core::vector3df(.0f, .0f, .0f), video::SColor(video::ECP_RED));
    floatingPieceOfShitNode = sceneManager->addCubeSceneNode(10.0f, 0, -1, core::vector3df(0, 0, 100));
    floatingPieceOfShitNode2 = sceneManager->addSphereSceneNode(5.0f, 4, 0, -1, core::vector3df(50, 50, 80));
}

void Game::error(const core::stringw &str) const
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
}

void Game::terminate()
{
    Config conf;
    conf.saveConfig("game.conf", configuration);
    device->closeDevice();
    device->run();
    device->drop();
}

void Game::menu()
{
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }
    configuration.resolution = driver->getScreenSize();
    initializeMenuGUI();

    while (device->run()) {
        if (eventReceiver->start){
            terminateMenuGUI();
            run();
            initializeMenuGUI();
        }
        if (eventReceiver->quit){
            break;
        }
        if (eventReceiver->settings){
            if (buttonStart != nullptr)
            {
                terminateMenuGUI();
                buttonStart = nullptr;
                initializeSettingsGUI();
            }
            if (eventReceiver->toggleFullscreen)
            {
                configuration.fullscreen = !configuration.fullscreen;
                terminate();
                configuration.resolution = core::dimension2d<u32>(1240, 720);
                customResolution = false;
                if (!initializeDevice())
                        return;
                initialized = true;
                initializeGUI();
                initializeMenuGUI();
            }
        }
        else if (buttonStart == nullptr)
            {
                terminateSettingsGUI();
                initializeMenuGUI();
            }
        if(eventReceiver->toggleGraphicMode)
        {
            switch (resolutionComboBox->getSelected())
            {
                case 0:
                    {
                        configuration.resolution = core::dimension2d<u32>(640, 480);
                        break;
                    }
                case 1:
                    {
                        configuration.resolution = core::dimension2d<u32>(1240, 720);
                        break;
                    }
                case 2:
                    {
                        customResolution = true;
                        break;
                    }
            }
            switch (colorDepthComboBox->getSelected())
            {
                case 0:
                {
                    configuration.colordepth = 8;
                    break;
                }
                case 1:
                {
                    configuration.colordepth = 16;
                    break;
                }
                case 2:
                {
                    configuration.colordepth = 32;
                    break;
                }
            }
            terminate();
            if (!initializeDevice())
                return;
            initialized = true;
            initializeGUI();
            initializeMenuGUI();
        }
        core::stringw scrs = "Screen size: ";
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        screenSizeText->setText(scrs.c_str());
        if(configuration.resolution != driver->getScreenSize())
        {
            if (!eventReceiver->settings){
                configuration.resolution = driver->getScreenSize();
                buttonWidth = configuration.resolution.Width / 6;
                if (buttonWidth < 180)
                    buttonWidth = 180;
                buttonHeight = buttonWidth * 1/8;
                buttonStart->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30));
                buttonSettings->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20));
                buttonQuit->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10));
            } else {
                configuration.resolution = driver->getScreenSize();
                buttonWidth = configuration.resolution.Width / 6;
                if (buttonWidth < 180)
                    buttonWidth = 180;
                buttonHeight = buttonWidth * 1/8;
                resolutionText->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 6*buttonHeight - 55));
                resolutionComboBox->setRelativePosition(core::position2di(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 6*buttonHeight - 60));
                colorDepthText->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 5*buttonHeight - 45));
                colorDepthComboBox->setRelativePosition(core::position2di(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 5*buttonHeight - 50));
                languageText->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 4*buttonHeight - 35));
                languageComboBox->setRelativePosition(core::position2di(configuration.resolution.Width - 3*buttonWidth/5 - 20, configuration.resolution.Height - 4*buttonHeight - 40));
                buttonToggleFullscreen->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30));
                buttonMenu->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20));
                buttonQuit->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10));
            }
        }
        device->getCursorControl()->setVisible(true);
        if (device->isWindowActive()) {
            driver->beginScene(true, true, video::SColor(0, 135, 206, 235));;
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }

    this->terminate();

}

void Game::run()
{

    initializeInGameGUI();
    initializeScene();
    bool escapePressed = false;
    configuration.resolution = driver->getScreenSize();

    while (device->run()) {
        if (eventReceiver->quit)
            break;
        if (!eventReceiver->start){
            terminateInGameGUI();
            break;
        }
        if (pause) {
            core::stringw scrs = "Screen size: ";
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            screenSizeText->setText(scrs.c_str());

            if (configuration.resolution != driver->getScreenSize()) {
                configuration.resolution = driver->getScreenSize();
                buttonWidth = configuration.resolution.Width / 6;
                if (buttonWidth < 180)
                    buttonWidth = 180;
                buttonHeight = buttonWidth * 1/8;
                buttonMenu->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20));
                buttonQuit->setRelativePosition(core::position2di(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10));
            }

            camera->setInputReceiverEnabled(false);

            buttonQuit->setVisible(true);
            buttonMenu->setVisible(true);
            screenSizeText->setVisible(true);
            cameraPosText->setVisible(false);
            device->getCursorControl()->setVisible(true);
        } else {
            core::stringw cameraPosition = "Camera position: (";
            core::vector3df position = camera->getPosition();
            cameraPosition += position.X;
            cameraPosition += ", ";
            cameraPosition += position.Y;
            cameraPosition += ", ";
            cameraPosition += position.Z;
            cameraPosition += ")";
            cameraPosText->setText(cameraPosition.c_str());

            camera->setInputReceiverEnabled(true);

            buttonQuit->setVisible(false);
            buttonMenu->setVisible(false);
            screenSizeText->setVisible(false);
            cameraPosText->setVisible(true);
            device->getCursorControl()->setVisible(false);
        }

        if (eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            if (!escapePressed) {
                pause = !pause;
                escapePressed = true;
            }
        } else if (!eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            escapePressed = false;
        }

        if (device->isWindowActive()) {
            driver->beginScene(true, true, video::SColor(0, 135, 206, 235));
            if (!pause)
                sceneManager->drawAll();
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }
    pause = false;
    sceneManager->clear();
}
