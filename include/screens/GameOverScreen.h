#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

using namespace irr;

class GameOverScreen : public IGUIScreen
{
public:
    GameOverScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~GameOverScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textMessage;
    gui::IGUIButton *buttonOK;
};

#endif // GAMEOVERSCREEN_H
