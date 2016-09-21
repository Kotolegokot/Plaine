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

struct ConfigData {
    bool fullscreen = false;
    core::dimension2d<u32> resolution = core::dimension2d<u32>(640, 480);;
    u32 colordepth = 32;
    core::stringw language = L"English";
};

class Config
{
public:
    Config() = default;
    ConfigData loadConfig(const std::string &filename);
    void saveConfig(const std::string &filename, const ConfigData &data);
};

#endif // CONFIG_H
