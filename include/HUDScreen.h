#ifndef HUDSCREEN_H
#define HUDSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class HUDScreen : public IGUIScreen
{
public:
    HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~HUDScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<std::weak_ptr<gui::IGUIElement>> getSelectableElements() override;

private:
    std::shared_ptr<gui::IGUIStaticText> textCameraPosition;
    std::shared_ptr<gui::IGUIStaticText> textObstaclesCount;
    std::shared_ptr<gui::IGUIStaticText> textFPS;
    std::shared_ptr<gui::IGUIStaticText> textVelocity;
    std::shared_ptr<gui::IGUIStaticText> textScore;
};

#endif // HUDSCREEN_H
