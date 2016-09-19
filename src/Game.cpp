#include "Game.h"

using namespace irr;

Game::Game()
{
    if (!initializeDevice())
        return;
    initializeScene();
    initialized = true;
}

Game::~Game()
{
    device->drop();
}

bool Game::initializeDevice(core::dimension2d<u32> windowSize)
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
    device->setResizable(true);

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

void Game::initializeInGameGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Menu", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
}

void Game::initializeSettingsGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonFullscreen = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 120, screenSize.Width - 100, screenSize.Height - 90), 0, ID_BUTTON_FULLSCREEN, L"Fullscreen", L"To Fullscreen Mode");
    buttonWindowed = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 100, screenSize.Height - 120, screenSize.Width - 20, screenSize.Height - 90), 0, ID_BUTTON_WINDOWED, L"Windowed", L"To Windewed Mode");
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 80, screenSize.Width - 20, screenSize.Height - 50), 0, ID_BUTTON_MENU, L"Back", L"Exit to Main menu");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 180, screenSize.Height - 40, screenSize.Width - 20, screenSize.Height - 10), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
    if (this->fullscreen)
        buttonFullscreen->setEnabled(false);
    else
        buttonWindowed->setEnabled(false);
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
            buttonStart->remove();
            buttonSettings->remove();
            buttonQuit->remove();
            screenSizeText->remove();
            this->run();
            this->initializeMenuGUI();
        }
        if (eventReceiver->quit){
            break;
        }
        if (eventReceiver->settings){
            if (buttonStart != nullptr)
            {
                buttonStart->remove();
                buttonSettings->remove();
                buttonQuit->remove();
                screenSizeText->remove();
                buttonStart = nullptr;
                this->initializeSettingsGUI();
            }
            if (eventReceiver->toggleFullscreen)
            {
                this->fullscreen = !this->fullscreen;
                this->terminate();
                if (!initializeDevice())
                        return;
                initializeScene();
                initialized = true;
                this->initializeMenuGUI();
            }
        }
        else if (buttonStart == nullptr)
            {
                buttonMenu->remove();
                buttonQuit->remove();
                buttonFullscreen->remove();
                buttonWindowed->remove();
                screenSizeText->remove();
                this->initializeMenuGUI();
            }
        core::stringw scrs = "Screen size: ";
        scrs += screenSize.Width;
        scrs += "x";
        scrs += screenSize.Height;
        screenSizeText->setText(scrs.c_str());

        if(screenSize != driver->getScreenSize())
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

void Game::terminate()
{
    device->drop();

    /**bool initialized = false;
    bool pause = false;

    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *sceneManager = nullptr;
    gui::IGUIEnvironment *guiEnvironment = nullptr;
    io::IFileSystem *fileSystem = nullptr;
    EventReceiver *eventReceiver = nullptr;

    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonMenu = nullptr;
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIStaticText *screenSizeText = nullptr;

    scene::ICameraSceneNode *camera = nullptr;
    scene::ILightSceneNode *light = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode = nullptr;
    scene::ISceneNode *floatingPieceOfShitNode2 = nullptr;**/

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
            buttonMenu->remove();
            buttonQuit->remove();
            screenSizeText->remove();
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
