#ifndef CONTROLSETTINGSSCREEN_H
#define CONTROLSETTINGSSCREEN_H

#include <array>
#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class ControlSettingsScreen : IGUIScreen
{
public:
    ControlSettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~ControlSettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<std::weak_ptr<gui::IGUIElement>> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    std::shared_ptr<gui::IGUIStaticText> textScreenSize;
    std::array<std::shared_ptr<gui::IGUIStaticText>, CONTROLS_COUNT> textsControl;
    std::array<std::shared_ptr<gui::IGUIButton>, CONTROLS_COUNT> buttonsControl;
    std::shared_ptr<gui::IGUIButton> buttonDefaultControls;
    std::shared_ptr<gui::IGUIButton> buttonSettings;
    std::shared_ptr<gui::IGUIButton> buttonQuit;
};

#endif // CONTROLSETTINGSSCREEN_H
