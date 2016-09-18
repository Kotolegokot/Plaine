#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;

class Game
{
public:
    Game();
    virtual ~Game();
    bool initialize();
    bool reinitialize();
    void terminate();

private:
    bool pause = false;
    bool initialized = false;
    IrrlichtDevice *device = nullptr;
    video::IVideoDriver *driver = nullptr;
    scene::ISceneManager *smgr = nullptr;
    gui::IGUIEnvironment *guienv = nullptr;
    io::IFileSystem *fs = nullptr;

    void error(const core::stringw &str);
};

#endif // GAME_H
