#ifndef CONTROLSETTINGSSCREEN_H
#define CONTROLSETTINGSSCREEN_H

#include <array>
#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class ControlSettingsScreen : public IGUIScreen
{
public:
    ControlSettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~ControlSettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textScreenSize;
    std::array<gui::IGUIStaticText *, CONTROLS_COUNT> textsControl;
    std::array<gui::IGUIButton *, CONTROLS_COUNT> buttonsControl;
    gui::IGUIButton *buttonDefaultControls;
    gui::IGUIButton *buttonSettings;
    gui::IGUIButton *buttonQuit;
};

#endif // CONTROLSETTINGSSCREEN_H
