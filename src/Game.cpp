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
        error(ERR_NOT_INITIALIZED);
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

    evRec = new EventReceiver(Context(&pause, device, driver, smgr, guienv, fs));
    device->setEventReceiver(evRec);

    // Menu
    buttonQuit = guienv->addButton(core::rect<s32>(20, 20, 100, 40), 0, ID_BUTTON_QUIT, L"Quit", L"Exits program");

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

    while (device->run()) {
        if (pause) {
            buttonQuit->setVisible(true);
        } else {
            buttonQuit->setVisible(false);
        }

        if (evRec->IsKeyDown(KEY_ESCAPE))
            pause = !pause;

        driver->beginScene(true, true, video::SColor(0, 3, 243, 250));
        if (!pause)
            smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }
}
