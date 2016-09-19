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
    device->setWindowCaption(L"Дерьмо");
    device->setResizable(true);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    fs = device->getFileSystem();

    evRec = new EventReceiver(Context(&pause, &quit, device));
    device->setEventReceiver(evRec);

    return true;
}

void Game::initializeGUI()
{
    core::dimension2du screenSize = driver->getScreenSize();
    screenSizeText = guienv->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonQuit = guienv->addButton(core::rect<s32>(screenSize.Width - 100, 10, screenSize.Width - 20, 30), 0, ID_BUTTON_QUIT, L"Quit", L"Exits program");
}

void Game::initializeScene()
{
    camera = smgr->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(0, 0, 30));

    light = smgr->addLightSceneNode(0, core::vector3df(.0f, .0f, .0f), video::SColor(video::ECP_RED));
    floatingPieceOfShitNode = smgr->addCubeSceneNode(10.0f, 0, -1, core::vector3df(0, 0, 100));
    floatingPieceOfShitNode2 = smgr->addSphereSceneNode(5.0f, 4, 0, -1, core::vector3df(50, 50, 80));
}

void Game::error(const core::stringw &str) const
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
}

void Game::run()
{
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }

    bool escapePressed = false;
    core::dimension2du screenSize = driver->getScreenSize();

    while (device->run()) {
        if (quit)
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

        if (evRec->IsKeyDown(KEY_ESCAPE)) {
            if (!escapePressed) {
                pause = !pause;
                escapePressed = true;
            }
        } else if (!evRec->IsKeyDown(KEY_ESCAPE)) {
            escapePressed = false;
        }

        if (evRec->IsKeyDown(KEY_KEY_W)) {
            core::vector3df dir = camera->getTarget() - camera->getAbsolutePosition();
            dir.normalize();
            camera->setPosition(camera->getPosition() + dir);
        } else if (evRec->IsKeyDown(KEY_KEY_S)) {
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
            driver->beginScene(true, true, video::SColor(0, 3, 243, 250));
            if (!pause)
                smgr->drawAll();
            guienv->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }

    device->drop();
}
