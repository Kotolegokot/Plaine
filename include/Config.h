#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <irrlicht.h>
#include "util.h"

using namespace irr;

struct Controls {
    EKEY_CODE up = KEY_KEY_W;
    EKEY_CODE down  =  KEY_KEY_S;
    EKEY_CODE left  =  KEY_KEY_A;
    EKEY_CODE right  =  KEY_KEY_D;
};

// structure containing all the configuration info
struct ConfigData {
    bool fullscreen = false;
    core::dimension2d<u32> resolution = core::dimension2d<u32>(640, 480);
    std::string language = "c";
    bool resizable = false;
    bool vsync = true;
    bool stencilBuffer = true;
    u32 renderDistance = 2000;
    Controls controls;
};

// this class helps save and load configuration files
// usually it is ./game.conf
//
// its format is easy:
//      var=expression
//      ...
// expression can be either a string ("bla-bla") or
//      a boolean (on/off) or an integer or
//      a float or a resolution (int,int)
// also controls are set like this:
//      controls:
//          key1=keyCode
//          key2=keyCode
//          ...
class Config
{
public:
    Config() = default;
    ConfigData loadConfig(const std::string &filename);
    void saveConfig(const std::string &filename, const ConfigData &data);
};

#endif // CONFIG_H
