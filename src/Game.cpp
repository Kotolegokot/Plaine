#include "Game.h"

using namespace irr;

Game::Game()
{
    initialize();
}

Game::~Game()
{
    terminate();
}

bool Game::initialize()
{
    if (initialized) {
        error(ERR_ALREADY_INITIALIZED);
        return false;
    }

    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1240, 720));
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
    device->setWindowCaption(L"Дерьмо");

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    fs = device->getFileSystem();

    evRec = new EventReceiver(Context(&pause, &quit, device));
    device->setEventReceiver(evRec);

    device->setResizable(true);

    // Menu
    core::dimension2du screenSize = driver->getScreenSize();
    buttonQuit = guienv->addButton(core::rect<s32>(screenSize.Width - 100, 10, screenSize.Width - 20, 30), 0, ID_BUTTON_QUIT, L"Quit", L"Exits program");
    screenSizeText = guienv->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), true);

    initialized = true;
}

bool Game::reinitialize()
{
    terminate();
    return initialize();
}

void Game::terminate()
{
    pause = false;
    initialized = false;
    quit = false;

    device->drop();
    device = nullptr;
    driver = nullptr;
    smgr = nullptr;
    guienv = nullptr;
    fs = nullptr;

    buttonQuit = nullptr;
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
                buttonQuit->setRelativePosition(core::position2di(screenSize.Width - 100, 10));
                screenSize = driver->getScreenSize();
            }
            buttonQuit->setVisible(true);
            screenSizeText->setVisible(true);
        } else {
            buttonQuit->setVisible(false);
            screenSizeText->setVisible(false);
        }

        if (evRec->IsKeyDown(KEY_ESCAPE)) {
            if (!escapePressed) {
                pause = !pause;
                escapePressed = true;
            }
        } else {
            escapePressed = false;
        }

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
