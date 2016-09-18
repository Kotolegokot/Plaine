#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <irrlicht/irrlicht.h>

using namespace irr;

struct Context {
    Context(bool *pause, bool *quit, IrrlichtDevice *device, video::IVideoDriver *driver, scene::ISceneManager *smgr,
        gui::IGUIEnvironment *guienv, io::IFileSystem *fs) :
            pause(pause), quit(quit), device(device), driver(driver), smgr(smgr), guienv(guienv), fs(fs)
    {}

    bool *pause;
    bool *quit;
    IrrlichtDevice *device;
    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;
    gui::IGUIEnvironment *guienv;
    io::IFileSystem *fs;
};

// GUI IDs
enum { ID_BUTTON_QUIT };

#endif // UTIL_H_INCLUDED
