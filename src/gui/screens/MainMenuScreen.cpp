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

#include "gui/screens/MainMenuScreen.h"

using namespace irr;

MainMenuScreen::MainMenuScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

MainMenuScreen::~MainMenuScreen()
{
    terminate();
}

void MainMenuScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    buttonStart = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonStart->setID(ID_BUTTON_START);
    setCustomButtonSkin(*buttonStart);

    buttonScoreboard = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonScoreboard->setID(ID_BUTTON_SCOREBOARD);
    setCustomButtonSkin(*buttonScoreboard);

    buttonSettings = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonSettings->setID(ID_BUTTON_SETTINGS);
    setCustomButtonSkin(*buttonSettings);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);

    initialized = true;
}

void MainMenuScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    buttonStart->setText(_wp("Start"));
    buttonStart->setToolTipText(_wp("Start game"));

    buttonScoreboard->setText(_wp("Scoreboard"));
    buttonScoreboard->setToolTipText(_wp("Scoreboard screen"));

    buttonSettings->setText(_wp("Settings"));
    buttonSettings->setToolTipText(_wp("Game settings"));

    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));
}

void MainMenuScreen::terminate()
{
    if (initialized) {
        buttonStart->remove();
        buttonScoreboard->remove();
        buttonSettings->remove();
        buttonQuit->remove();

        initialized = false;
    }
}

void MainMenuScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    buttonStart->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                     configuration.resolution.Height - 4 * buttonHeight - 4 * SPACE,
                                                     configuration.resolution.Width - 2 * SPACE,
                                                     configuration.resolution.Height - 3 * buttonHeight - 4 * SPACE));

    buttonScoreboard->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                          configuration.resolution.Height - 3 * buttonHeight - 3 * SPACE,
                                                          configuration.resolution.Width - 2 * SPACE,
                                                          configuration.resolution.Height - 2 * buttonHeight - 3 * SPACE));

    buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                        configuration.resolution.Height - 2 * buttonHeight - 2 * SPACE,
                                                        configuration.resolution.Width - 2 * SPACE,
                                                        configuration.resolution.Height - buttonHeight - 2 * SPACE));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                    configuration.resolution.Height - buttonHeight - SPACE,
                                                    configuration.resolution.Width - 2 * SPACE,
                                                    configuration.resolution.Height - SPACE));
}

std::vector<gui::IGUIElement *> MainMenuScreen::getSelectableElements()
{
    return { buttonStart, buttonScoreboard, buttonSettings, buttonQuit };
}

void MainMenuScreen::setVisible(bool visible)
{
    buttonStart->setVisible(visible);
    buttonScoreboard->setVisible(visible);
    buttonSettings->setVisible(visible);
    buttonQuit->setVisible(visible);
}
