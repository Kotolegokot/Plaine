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

struct ConfigData {
    bool fullscreen = false;
    core::dimension2d<u32> resolution = core::dimension2d<u32>(640, 480);;
    u32 colordepth = 32;
    std::string language = "c";
    bool resizable = false;
    bool vsync = true;
    bool stencilBuffer = true;
    Controls controls;
};

class Config
{
public:
    Config() = default;
    ConfigData loadConfig(const std::string &filename);
    void saveConfig(const std::string &filename, const ConfigData &data);
};

#endif // CONFIG_H
