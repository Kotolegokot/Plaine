#ifndef PAUSEMENUSCREEN_H
#define PAUSEMENUSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class PauseMenuScreen : public IGUIScreen
{
public:
    PauseMenuScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~PauseMenuScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<std::weak_ptr<gui::IGUIElement>> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

private:
    std::shared_ptr<gui::IGUIStaticText> textScreenSize;
    std::shared_ptr<gui::IGUIButton> buttonResume;
    std::shared_ptr<gui::IGUIButton> buttonMenu;
    std::shared_ptr<gui::IGUIButton> buttonQuit;
};

#endif // PAUSEMENUSCREEN_H
