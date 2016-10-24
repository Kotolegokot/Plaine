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
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText* textScreenSize;
    gui::IGUIButton* buttonStart;
    gui::IGUIButton* buttonSettings;
    gui::IGUIButton* buttonQuit;
};

#endif // MAINMENUSCREEN_H
