#include "Game.h"

using namespace irr;

Game::Game()
{
    windowSize = core::dimension2d<u32>(1240, 720);
    if (!initializeDevice())
        return;
    initializeScene();
    initialized = true;
}

Game::~Game()
{
    device->drop();
}

bool Game::initializeDevice()
{
    if (this->fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        windowSize = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }
    device = createDevice(video::EDT_OPENGL, windowSize, 32, this->fullscreen);
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
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Menu", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
}

void Game::terminateInGameGUI()
{
    buttonMenu->remove();
    buttonQuit->remove();
    screenSizeText->remove();
}

void Game::initializeSettingsGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonFullscreen = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 120, screenSize.Width - 100, screenSize.Height - 90), 0, ID_BUTTON_FULLSCREEN, L"Fullscreen", L"To Fullscreen Mode");
    buttonWindowed = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 100, screenSize.Height - 120, screenSize.Width - 20, screenSize.Height - 90), 0, ID_BUTTON_WINDOWED, L"Windowed", L"To Windewed Mode");
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Back", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
    resolutionComboBox = guiEnvironment->addComboBox(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 150, screenSize.Width - 20, screenSize.Height - 130), 0, ID_RESOLUTION_COMBOBOX);
    if (this->fullscreen)
    {
        buttonFullscreen->setEnabled(false);
        resolutionComboBox->setEnabled(false);
    }
    else
        buttonWindowed->setEnabled(false);
    resolutionComboBox->addItem(L"640x480", 0);
    resolutionComboBox->addItem(L"1240x720", 1);
    if (windowSize == core::dimension2d<u32>(640, 480))
        resolutionComboBox->setSelected(0);
    else if (windowSize == core::dimension2d<u32>(1240, 720))
        resolutionComboBox->setSelected(1);
}

void Game::terminateSettingsGUI()
{
    buttonMenu->remove();
    buttonQuit->remove();
    buttonFullscreen->remove();
    buttonWindowed->remove();
    screenSizeText->remove();
    resolutionComboBox->remove();
}

void Game::initializeScene()
{
    camera = sceneManager->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(0, 0, 30));

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
            this->terminateMenuGUI();
            this->run();
            this->initializeMenuGUI();
        }
        if (eventReceiver->quit){
            break;
        }
        if (eventReceiver->settings){
            if (buttonStart != nullptr)
            {
                this->terminateMenuGUI();
                buttonStart = nullptr;
                this->initializeSettingsGUI();
            }
            if (eventReceiver->toggleFullscreen)
            {
                this->fullscreen = !this->fullscreen;
                this->terminate();
                this->windowSize = core::dimension2d<u32>(1240, 720);
                if (!initializeDevice())
                        return;
                initializeScene();
                initialized = true;
                this->initializeMenuGUI();
            }
        }
        else if (buttonStart == nullptr)
            {
                this->terminateSettingsGUI();
                this->initializeMenuGUI();
            }
        if(eventReceiver->toggleResolution)
        {
            switch (resolutionComboBox->getSelected())
            {
                case 0:
                    {
                        this->terminate();
                        windowSize = core::dimension2d<u32>(640, 480);
                        if (!initializeDevice())
                            return;
                        initializeScene();
                        initialized = true;
                        this->initializeMenuGUI();
                        break;
                    }
                case 1:
                    {
                        this->terminate();
                        windowSize = core::dimension2d<u32>(1240, 720);
                        if (!initializeDevice())
                            return;
                        initializeScene();
                        initialized = true;
                        this->initializeMenuGUI();
                        break;
                    }
            }
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
            }
            else {
                screenSize = driver->getScreenSize();
                buttonFullscreen->setRelativePosition(core::position2di(screenSize.Width - 180, screenSize.Height - 120));
                buttonWindowed->setRelativePosition(core::position2di(screenSize.Width - 100, screenSize.Height - 120));
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

    this->initializeInGameGUI();
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

            buttonQuit->setVisible(true);
            buttonMenu->setVisible(true);
            screenSizeText->setVisible(true);

            device->getCursorControl()->setVisible(true);
        } else {
            buttonQuit->setVisible(false);
            buttonMenu->setVisible(false);
            screenSizeText->setVisible(false);
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

        if (eventReceiver->IsKeyDown(KEY_KEY_W)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            camera->setPosition(camera->getPosition() + dir);
        } else if (eventReceiver->IsKeyDown(KEY_KEY_S)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            camera->setPosition(camera->getPosition() - dir);
        }

        /*if (evRec->IsKeyDown(KEY_KEY_A)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            dir.rotateXZBy(90);
            camera->setPosition(camera->getPosition() + dir);
        } else if (evRec->IsKeyDown(KEY_KEY_D)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            dir.rotateXZBy(90);
            camera->setPosition(camera->getPosition() - dir);
        }

        if (evRec->IsKeyDown(KEY_SPACE)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            dir.rotateYZBy(90);
            camera->setPosition(camera->getPosition() - dir);
        } else if (evRec->IsKeyDown(KEY_LSHIFT)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            dir.rotateYZBy(90);
            camera->setPosition(camera->getPosition() + dir);
        }*/



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
}
