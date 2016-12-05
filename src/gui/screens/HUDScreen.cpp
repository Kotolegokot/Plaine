/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/screens/HUDScreen.h"

HUDScreen::HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

HUDScreen::~HUDScreen()
{
    terminate();
}

void HUDScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textCameraPosition = guiEnvironment.addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30));
    textCameraPosition->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textObstaclesCount = guiEnvironment.addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24));
    textObstaclesCount->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textFPS = guiEnvironment.addStaticText(L"FPS", core::rect<s32>(10, 10 + 24*2, 400, 30 + 24*2));
    textFPS->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textVelocity = guiEnvironment.addStaticText(L"VELOCITY", core::rect<s32>(10, 10 + 24*3, 400, 30 + 24*3));
    textVelocity->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textAngle = guiEnvironment.addStaticText(L"ANGLE", core::rect<s32>(10, 10 + 24*4, 400, 30 + 24*4));
    textAngle->setBackgroundColor(video::SColor(120, 255, 255, 255));

    textScore = guiEnvironment.addStaticText(L"POINTS", core::rect<s32>(10, 10 + 24*5, 400, 30 + 24*5));
    textScore->setBackgroundColor(video::SColor(120, 255, 255, 255));

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
}

void HUDScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    textCameraPosition->setVisible(allVisible && infoVisible);
    textObstaclesCount->setVisible(allVisible && infoVisible);
    textFPS->setVisible(allVisible && infoVisible);
    textVelocity->setVisible(allVisible && infoVisible);
    textAngle->setVisible(allVisible && infoVisible);
    textScore->setVisible(allVisible && infoVisible);
}

void HUDScreen::terminate()
{
    textCameraPosition->remove();
    textObstaclesCount->remove();
    textFPS->remove();
    textVelocity->remove();
    textAngle->remove();
    textScore->remove();
}

void HUDScreen::resize(s32, s32) {}

std::vector<gui::IGUIElement *> HUDScreen::getSelectableElements()
{
    return {};
}

void HUDScreen::setVisible(bool visible)
{
    allVisible = visible;
}

void HUDScreen::setInfoVisible(bool visible)
{
    infoVisible = visible;
}

bool HUDScreen::getInfoVisible() const
{
    return infoVisible;
}
