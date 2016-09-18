#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht/irrlicht.h>
#include "EventReceiver.h"
#include "util.h"

using namespace irr;

class Game
{
public:
    Game();
    ~Game();
    bool initialize();
    bool reinitialize();
    void terminate();
    void run();

private:
    bool pause = false;
    bool initialized = false;
    bool quit = false;
    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *smgr = nullptr;
    gui::IGUIEnvironment *guienv = nullptr;
    io::IFileSystem *fs = nullptr;
    EventReceiver *evRec = nullptr;

    gui::IGUIButton *buttonQuit = nullptr;

    void error(const core::stringw &str) const;

    const core::stringw ERR_NOT_INITIALIZED = "game is not initialized!";
    const core::stringw ERR_ALREADY_INITIALIZED = "game is already initialized!";
};

#endif // GAME_H
