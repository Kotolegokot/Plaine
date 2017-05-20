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

#include "gui/screens/PauseMenuScreen.h"

PauseMenuScreen::PauseMenuScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

PauseMenuScreen::~PauseMenuScreen()
{
    terminate();
}

void PauseMenuScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    guiEnvironment.getSkin()->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,230,230,230));

    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE",core::rect<s32>(10, 10, 200, 30));

    buttonResume = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonResume->setID(ID_BUTTON_RESUME);
    setCustomButtonSkin(*buttonResume);

    buttonMenu = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonMenu->setID(ID_BUTTON_MENU);
    setCustomButtonSkin(*buttonMenu);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);

    initialized = true;
}

void PauseMenuScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    buttonResume->setText(_wp("Resume"));
    buttonResume->setToolTipText(_wp("Resume game"));

    buttonMenu->setText(_wp("Menu"));
    buttonMenu->setToolTipText(_wp("Exit to main menu"));

    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));
}

void PauseMenuScreen::terminate()
{
    if (initialized) {
        textScreenSize->remove();
        buttonResume->remove();
        buttonMenu->remove();
        buttonQuit->remove();

        initialized = false;
    }
}

void PauseMenuScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    buttonResume->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                      configuration.resolution.Height - 3 * buttonHeight - 3 * SPACE,
                                                      configuration.resolution.Width - 2 * SPACE,
                                                      configuration.resolution.Height - 2 * buttonHeight - 3 * SPACE));

    buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                    configuration.resolution.Height - 2 * buttonHeight - 2 * SPACE,
                                                    configuration.resolution.Width - 2 * SPACE,
                                                    configuration.resolution.Height - buttonHeight - 2 * SPACE));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                    configuration.resolution.Height - buttonHeight - SPACE,
                                                    configuration.resolution.Width - 2 * SPACE,
                                                    configuration.resolution.Height - SPACE));
}

std::vector<gui::IGUIElement *> PauseMenuScreen::getSelectableElements()
{
    return { buttonResume, buttonMenu, buttonQuit };
}

void PauseMenuScreen::setVisible(bool visible)
{
    textScreenSize->setVisible(visible);
    buttonResume->setVisible(visible);
    buttonMenu->setVisible(visible);
    buttonQuit->setVisible(visible);
}
