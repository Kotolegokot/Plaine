#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

using namespace irr;

class MainMenuScreen : public IGUIScreen
{
public:
    MainMenuScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~MainMenuScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<std::weak_ptr<gui::IGUIElement>> getSelectableElements() override;

private:
    std::shared_ptr<gui::IGUIStaticText> textScreenSize;
    std::shared_ptr<gui::IGUIButton> buttonStart;
    std::shared_ptr<gui::IGUIButton> buttonSettings;
    std::shared_ptr<gui::IGUIButton> buttonQuit;
};

#endif // MAINMENUSCREEN_H
