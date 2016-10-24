#include "screens/HUDScreen.h"

HUDScreen::HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

HUDScreen::~HUDScreen()
{
    terminate();
}

void HUDScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textCameraPosition = guiEnvironment.addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    textCameraPosition->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textObstaclesCount = guiEnvironment.addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24), false);
    textObstaclesCount->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textFPS = guiEnvironment.addStaticText(L"FPS", core::rect<s32>(10, 10 + 24 + 24, 400, 30 + 24 + 24), false);
    textFPS->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textVelocity = guiEnvironment.addStaticText(L"VELOCITY", core::rect<s32>(10, 10 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24), false);
    textVelocity->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textScore = guiEnvironment.addStaticText(L"POINTS", core::rect<s32>(10, 10 + 24 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24 + 24), false);
    textScore->setBackgroundColor(video::SColor(120, 255, 255, 255));

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
}

void HUDScreen::reload(s32 buttonWidth, s32 buttonHeight)
{
    if (!infoVisible) {
        textCameraPosition->setVisible(false);
        textObstaclesCount->setVisible(false);
        textFPS->setVisible(false);
        textVelocity->setVisible(false);
        textScore->setVisible(false);
    }
}

void HUDScreen::terminate()
{
    textCameraPosition->remove();
    textObstaclesCount->remove();
    textFPS->remove();
    textVelocity->remove();
    textScore->remove();
}

void HUDScreen::resize(s32, s32) {}

std::vector<gui::IGUIElement *> HUDScreen::getSelectableElements()
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
