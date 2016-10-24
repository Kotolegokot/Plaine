#ifndef IGUISCREEN_H
#define IGUISCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <irrlicht.h>
#include "Config.h"

using namespace irr;

class IGUIScreen
{
public:
    IGUIScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
        configuration(configuration), guiEnvironment(guiEnvironment) {}
    virtual ~IGUIScreen() {}

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void terminate() = 0;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() = 0;
    virtual void setVisible(bool visible) = 0;

protected:
    const ConfigData &configuration;
    gui::IGUIEnvironment &guiEnvironment;

    static constexpr s32 space = 10;
};

#endif // IGUISCREEN_H
