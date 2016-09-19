#include "Game.h"

using namespace irr;

Game::Game()
{
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

bool Game::initializeDevice()
{
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1240, 720));
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

void Game::initializeGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 100, 10, screenSize.Width - 20, 30), 0, ID_BUTTON_START, L"Start", L"Start game");
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(screenSize.Width - 100, 40, screenSize.Width - 20, 60), 0, ID_BUTTON_QUIT, L"Quit", L"Exit game");
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
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }

    core::dimension2du screenSize = driver->getScreenSize();

    while (device->run()) {
        if (eventReceiver->quit){
            break;
        }
        if (eventReceiver->start){
            buttonStart->setVisible(false);
            buttonQuit->setVisible(false);
            screenSizeText->setVisible(false);
            this->run();
            break;
        }
        core::stringw scrs = "Screen size: ";
        scrs += screenSize.Width;
        scrs += "x";
        scrs += screenSize.Height;
        screenSizeText->setText(scrs.c_str());

        if (screenSize != driver->getScreenSize()) {
            screenSize = driver->getScreenSize();
            buttonStart->setRelativePosition(core::position2di(screenSize.Width - 100, 10));
            buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 100, 40));
        }

        buttonStart->setVisible(true);
        buttonQuit->setVisible(true);
        screenSizeText->setVisible(true);

        device->getCursorControl()->setVisible(true);
        if (device->isWindowActive()) {
            driver->beginScene(true, true, video::SColor(0, 135, 206, 235));;
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }

    device->drop();

}

void Game::run()
{

    bool escapePressed = false;
    core::dimension2du screenSize = driver->getScreenSize();

    while (device->run()) {
        if (eventReceiver->quit)
            break;

        if (pause) {
            core::stringw scrs = "Screen size: ";
            scrs += screenSize.Width;
            scrs += "x";
            scrs += screenSize.Height;
            screenSizeText->setText(scrs.c_str());

            if (screenSize != driver->getScreenSize()) {
                screenSize = driver->getScreenSize();
                buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 100, 10));
            }

            buttonQuit->setVisible(true);
            screenSizeText->setVisible(true);

            device->getCursorControl()->setVisible(true);
        } else {
            buttonQuit->setVisible(false);
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

    device->drop();
}
