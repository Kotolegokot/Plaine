#include "Config.h"

using namespace irr;

ConfigData Config::loadConfig(const std::string &filename)
{
    ConfigData data;
}

void Config::saveConfig(const std::string &filename, const ConfigData &data)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for writing" << std::endl;
        return;
    }

    outputFile << "resolution=" << data.resolution.Width << "x" << data.resolution.Height << std::endl;
    outputFile << "fullscreen=" << (data.fullscreen ? "on" : "off") << std::endl;
    outputFile << "colordepth=" << data.colordepth << std::endl;
    outputFile << "language=" << wide_to_utf8(std::wstring(data.language.c_str())) << std::endl;
}
