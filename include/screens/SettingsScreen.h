#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class SettingsScreen : public IGUIScreen
{
public:
    SettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~SettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<std::weak_ptr<gui::IGUIElement>> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

private:
    std::shared_ptr<gui::IGUIStaticText> textScreenSize;
    std::shared_ptr<gui::IGUIStaticText> textLanguage;
    std::shared_ptr<gui::IGUIStaticText> textResolution;
    std::shared_ptr<gui::IGUIStaticText> textRenderDistance;
    std::shared_ptr<gui::IGUIComboBox> comboBoxLanguage;
    std::shared_ptr<gui::IGUIComboBox> comboBoxResolution;
    std::shared_ptr<gui::IGUISpinBox> spinBoxRenderDistance;
    std::shared_ptr<gui::IGUICheckBox> checkBoxVSync;
    std::shared_ptr<gui::IGUICheckBox> checkBoxStencilBuffer;
    std::shared_ptr<gui::IGUIButton> buttonToggleFullscreen;
    std::shared_ptr<gui::IGUIButton> buttonControlSettings;
    std::shared_ptr<gui::IGUIButton> buttonMenu;
    std::shared_ptr<gui::IGUIButton> buttonQuit;
};

#endif // SETTINGSSCREEN_H
