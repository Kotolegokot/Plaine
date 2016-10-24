#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

using namespace irr;

class SettingsScreen : public IGUIScreen
{
public:
    SettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~SettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textScreenSize;
    gui::IGUIStaticText *textLanguage;
    gui::IGUIStaticText *textResolution;
    gui::IGUIStaticText *textRenderDistance;
    gui::IGUIComboBox *comboBoxLanguage;
    gui::IGUIComboBox *comboBoxResolution;
    gui::IGUISpinBox *spinBoxRenderDistance;
    gui::IGUICheckBox *checkBoxVSync;
    gui::IGUICheckBox *checkBoxStencilBuffer;
    gui::IGUIButton *buttonToggleFullscreen;
    gui::IGUIButton *buttonControlSettings;
    gui::IGUIButton *buttonMenu;
    gui::IGUIButton *buttonQuit;
};

#endif // SETTINGSSCREEN_H
