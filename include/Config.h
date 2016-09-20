#ifndef CONFIG_H
#define CONFIG_H

#include <irrlicht/irrlicht.h>

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
    ConfigData loadConfig(const core::stringc &filename);
    void saveConfig(const core::stringc &filename, const ConfigData &data);
};

#endif // CONFIG_H
