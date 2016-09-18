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
    if (initialized)
        return false;

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

    driver->drop();
    driver = nullptr;

    smgr->drop();
    smgr = nullptr;

    guienv->drop();
    guienv = nullptr;

    fs->drop();
    fs = nullptr;
}

void Game::error(const core::stringw &str)
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
}
