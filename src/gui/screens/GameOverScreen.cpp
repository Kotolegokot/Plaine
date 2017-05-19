/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/screens/GameOverScreen.h"

using namespace irr;

GameOverScreen::GameOverScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

GameOverScreen::~GameOverScreen()
{
    terminate();
}

void GameOverScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    guiEnvironment.getSkin()->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,230,230,230));
    textMessage = guiEnvironment.addStaticText(L"LOSER!!1", core::rect<s32>(0, 0, 0, 0));
    textScore = guiEnvironment.addStaticText(L"SCORE", core::rect<s32>(0, 0, 0, 0));

    buttonOK = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonOK->setID(ID_BUTTON_MENU);
    setCustomButtonSkin(*buttonOK);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);

    initialized = true;
}

void GameOverScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    textMessage->setText(_wp("Losing is fun!"));
    buttonOK->setText(_wp("OK"));
}

void GameOverScreen::terminate()
{
    if (initialized) {
        textMessage->remove();
        textScore->remove();
        buttonOK->remove();

        initialized = false;
    }
}

void GameOverScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    textMessage->setRelativePosition(core::rect<s32>(configuration.resolution.Width / 2 - 100,
                                                     configuration.resolution.Height / 2,
                                                     configuration.resolution.Width / 2 + 100,
                                                     configuration.resolution.Height / 2 + 20));

    textScore->setRelativePosition(core::rect<s32>(configuration.resolution.Width / 2 - 100,
                                                   configuration.resolution.Height / 2 + 24,
                                                   configuration.resolution.Width / 2 + 100,
                                                   configuration.resolution.Height / 2 + 20 + 24));

    buttonOK->setRelativePosition(core::rect<s32>(configuration.resolution.Width / 2- buttonWidth / 2,
                                                    configuration.resolution.Height / 2 + 20*2 + 24*2,
                                                    configuration.resolution.Width / 2 + buttonWidth / 2,
                                                    configuration.resolution.Height / 2 + 20*2 + 24*2 + buttonHeight));
}

std::vector<gui::IGUIElement *> GameOverScreen::getSelectableElements()
{
    return { buttonOK };
}

void GameOverScreen::setVisible(bool visible)
{
    textMessage->setVisible(visible);
    textScore->setVisible(visible);
    buttonOK->setVisible(visible);
}
