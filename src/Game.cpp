#include "Game.h"

using namespace irr;

Game::Game()
{
    windowSize = core::dimension2d<u32>(1240, 720);
    colorDepth = 32;
    language = L"English";
    if (!initializeDevice())
        return;
    initializeGUI();
    initializeScene();
    initialized = true;
}

Game::Game(struct ConfigData &data)
{
    windowSize = data.resolution;
    fullscreen = data.fullscreen;
    colorDepth = data.colordepth;
    language = data.language;
    if (!initializeDevice())
        return;
    initializeGUI();
    initializeScene();
    initialized = true;
}

Game::~Game()
{
    device->drop();
}

void Game::initializeGUI()
{
    gui::IGUISkin *skin = guiEnvironment->getSkin();
    gui::IGUIFont *font = guiEnvironment->getFont("media/fonts/fonthaettenschweiler.bmp");
    if (font)
        skin->setFont(font);
}

bool Game::initializeDevice()
{
    if (fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        windowSize = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }
    device = createDevice(video::EDT_OPENGL, windowSize, colorDepth, fullscreen);
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
    device->setResizable(customScreenSize);
    return true;
}

void Game::initializeMenuGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 120, screenSize.Width - 20, screenSize.Height - 90), 0, ID_BUTTON_START, L"Start", L"Start game");
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_SETTINGS, L"Settings", L"Game settings");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
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
    core::dimension2du screenSize = driver->getScreenSize();
    cameraPosText = guiEnvironment->addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Menu", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
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
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    resolutionText = guiEnvironment->addStaticText(L"Resolution:", core::rect<s32>(screenSize.Width - 180, screenSize.Height - 190, screenSize.Width - 135, screenSize.Height - 170), false);
    resolutionComboBox = guiEnvironment->addComboBox(core::rect<s32>(screenSize.Width - 130, screenSize.Height - 195, screenSize.Width - 20, screenSize.Height - 175), 0, ID_RESOLUTION_COMBOBOX);
    if (fullscreen)
    {
        resolutionComboBox->setEnabled(false);
        resolutionComboBox->addItem(L"Fullscreen", 0);
        resolutionComboBox->setSelected(0);
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 120, screenSize.Width - 20, screenSize.Height - 90), 0, ID_BUTTON_TOGGLE_FULLSCREEN, L"Windowed", L"To Windewed Mode");

    }
    else {
        resolutionComboBox->addItem(L"640x480", 0);
        resolutionComboBox->addItem(L"1240x720", 1);
        resolutionComboBox->addItem(L"Custom Resolution", 2);
        if (windowSize == core::dimension2d<u32>(640, 480))
            resolutionComboBox->setSelected(0);
        else if (windowSize == core::dimension2d<u32>(1240, 720))
            resolutionComboBox->setSelected(1);
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 120, screenSize.Width - 20, screenSize.Height - 90), 0, ID_BUTTON_TOGGLE_FULLSCREEN, L"Fullscreen", L"To Fullscreen Mode");

    }
    colorDepthText = guiEnvironment->addStaticText(L"Color Depth:", core::rect<s32>(screenSize.Width - 180, screenSize.Height - 165, screenSize.Width - 130, screenSize.Height - 145), false);
    colorDepthComboBox = guiEnvironment->addComboBox(core::rect<s32>(screenSize.Width - 130, screenSize.Height - 170, screenSize.Width - 20, screenSize.Height - 150), 0, ID_COLORDEPTH_COMBOBOX);
    colorDepthComboBox->addItem(L"8", 0);
    colorDepthComboBox->addItem(L"16", 1);
    colorDepthComboBox->addItem(L"32", 2);
    if (colorDepth == 8)
        colorDepthComboBox->setSelected(0);
    else if (colorDepth == 16)
        colorDepthComboBox->setSelected(1);
    else if (colorDepth == 32)
        colorDepthComboBox->setSelected(2);
    languageText = guiEnvironment->addStaticText(L"Language:", core::rect<s32>(screenSize.Width - 180, screenSize.Height - 140, screenSize.Width - 130, screenSize.Height - 120), false);
    languageComboBox = guiEnvironment->addComboBox(core::rect<s32>(screenSize.Width - 130, screenSize.Height - 145, screenSize.Width - 20, screenSize.Height - 125), 0, ID_LANGUAGE_COMBOBOX);
    languageComboBox->addItem(L"English", 0);
    if (language == L"English")
        languageComboBox->setSelected(0);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Back", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
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
    device->closeDevice();
    device->run();
    device->drop();
}

void Game::menu()
{
    this->initializeMenuGUI();
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }
    core::dimension2du screenSize = driver->getScreenSize();

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
                fullscreen = !fullscreen;
                terminate();
                windowSize = core::dimension2d<u32>(1240, 720);
                customScreenSize = false;
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
                        windowSize = core::dimension2d<u32>(640, 480);
                        break;
                    }
                case 1:
                    {
                        windowSize = core::dimension2d<u32>(1240, 720);
                        break;
                    }
                case 2:
                    {
                        customScreenSize = true;
                        break;
                    }
            }
            switch (colorDepthComboBox->getSelected())
            {
                case 0:
                {
                    colorDepth = 8;
                    break;
                }
                case 1:
                {
                    colorDepth = 16;
                    break;
                }
                case 2:
                {
                    colorDepth = 32;
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
        scrs += screenSize.Width;
        scrs += "x";
        scrs += screenSize.Height;
        screenSizeText->setText(scrs.c_str());
        if(screenSize != driver->getScreenSize())
        {
            if (!eventReceiver->settings){
                screenSize = driver->getScreenSize();
                buttonStart->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 120));
                buttonSettings->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 80));
                buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 40));
            } else {
                screenSize = driver->getScreenSize();
                resolutionText->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 190));
                resolutionComboBox->setRelativePosition(core::position2di(screenSize.Width - 130, screenSize.Height - 195));
                colorDepthText->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 165));
                colorDepthComboBox->setRelativePosition(core::position2di(screenSize.Width - 130, screenSize.Height - 170));
                languageText->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 140));
                languageComboBox->setRelativePosition(core::position2di(screenSize.Width - 130, screenSize.Height - 145));
                buttonToggleFullscreen->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 120));
                buttonMenu->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 80));
                buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 40));
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
    core::dimension2du screenSize = driver->getScreenSize();

    while (device->run()) {
        if (eventReceiver->quit)
            break;
        if (!eventReceiver->start){
            terminateInGameGUI();
            break;
        }
        if (pause) {
            core::stringw scrs = "Screen size: ";
            scrs += screenSize.Width;
            scrs += "x";
            scrs += screenSize.Height;
            screenSizeText->setText(scrs.c_str());

            if (screenSize != driver->getScreenSize()) {
                screenSize = driver->getScreenSize();
                buttonMenu->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 80));
                buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 40));
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
