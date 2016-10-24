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

#include "GameOverScreen.h"

using namespace irr;

GameOverScreen::GameOverScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

GameOverScreen::~GameOverScreen()
{
    terminate();
}

void GameOverScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textMessage = guiEnvironment.addStaticText(L"LOSER!!1", core::rect<s32>(0, 0, 0, 0));

    buttonOK = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonOK->setID(ID_BUTTON_MENU);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
}

void GameOverScreen::reload(s32 buttonWidth, s32 buttonHeight)
{
    textMessage->setText(_wp("Losing is fun!"));
    buttonOK->setText(_wp("OK"));
}

void GameOverScreen::terminate()
{
    textMessage->remove();
    buttonOK->remove();
}

void GameOverScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    textMessage->setRelativePosition(core::rect<s32>(configuration.resolution.Width / 2 - 200,
                                                     configuration.resolution.Height / 2,
                                                     configuration.resolution.Width / 2 + 200,
                                                     configuration.resolution.Height / 2 + 20));

    buttonOK->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - space));
}

std::vector<gui::IGUIElement *> GameOverScreen::getSelectableElements()
{
    return { buttonOK };
}

void GameOverScreen::setVisible(bool visible)
{
    textMessage->setVisible(visible);
    buttonOK->setVisible(visible);
}
