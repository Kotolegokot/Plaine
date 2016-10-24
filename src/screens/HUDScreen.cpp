#include "HUDScreen.h"

HUDScreen::HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

HUDScreen::~HUDScreen()
{
    terminate();
}

void HUDScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textCameraPosition = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false));
    textCameraPosition->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textObstaclesCount = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24), false));
    textObstaclesCount->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textFPS = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"FPS", core::rect<s32>(10, 10 + 24 + 24, 400, 30 + 24 + 24), false));
    textFPS->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textVelocity = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"VELOCITY", core::rect<s32>(10, 10 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24), false));
    textVelocity->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textScore = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"POINTS", core::rect<s32>(10, 10 + 24 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24 + 24), false));
    textScore->setBackgroundColor(video::SColor(120, 255, 255, 255));

    if (!infoVisible) {
        textCameraPosition->setVisible(false);
        textObstaclesCount->setVisible(false);
        textFPS->setVisible(false);
        textVelocity->setVisible(false);
        textScore->setVisible(false);
    }

    resize(buttonWidth, buttonHeight);
}

void HUDScreen::terminate()
{
    textCameraPosition->remove();
    textCameraPosition.reset();

    textObstaclesCount->remove();
    textObstaclesCount.reset();

    textFPS->remove();
    textFPS.reset();

    textVelocity->remove();
    textVelocity.reset();

    textScore->remove();
    textScore.reset();
}

void HUDScreen::resize(s32, s32) {}

std::vector<std::weak_ptr<gui::IGUIElement>> HUDScreen::getSelectableElements()
{
    return {};
}

void HUDScreen::setVisible(bool visible)
{
    textCameraPosition->setVisible(visible);
    textObstaclesCount->setVisible(visible);
    textFPS->setVisible(visible);
    textVelocity->setVisible(visible);
    textScore->setVisible(visible);
}

void HUDScreen::setInfoVisible(bool visible)
{
    infoVisible = visible;
}

bool HUDScreen::getInfoVisible() const
{
    return infoVisible;
}
