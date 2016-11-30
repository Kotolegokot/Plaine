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

#include "gui/screens/ControlSettingsScreen.h"

ControlSettingsScreen::ControlSettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

ControlSettingsScreen::~ControlSettingsScreen()
{
    terminate();
}

void ControlSettingsScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(0, 0, 0, 0));

    static constexpr std::array<u32, CONTROLS_COUNT>
            ids { { ID_BUTTON_CONTROL_UP, ID_BUTTON_CONTROL_LEFT,
                    ID_BUTTON_CONTROL_DOWN, ID_BUTTON_CONTROL_RIGHT,
                    ID_BUTTON_CONTROL_CW_ROLL, ID_BUTTON_CONTROL_CCW_ROLL } };
    for (std::size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i] = guiEnvironment.addStaticText(L"CONTROL", core::rect<s32>(0, 0, 0, 0));

        buttonsControl[i] = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
        buttonsControl[i]->setID(ids[i]);
        setCustomButtonSkin(*buttonsControl[i], true);
    }

    buttonDefaultControls = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonDefaultControls->setID(ID_BUTTON_DEFAULT_CONTROLS);
    setCustomButtonSkin(*buttonDefaultControls);

    buttonSettings = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonSettings->setID(ID_BUTTON_SETTINGS);
    setCustomButtonSkin(*buttonSettings);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
}

void ControlSettingsScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    static constexpr std::array<const char *, CONTROLS_COUNT>
            texts { { "Up: ", "Left: ", "Down: ", "Right: ", "CW roll", "CCW roll" } };
    for (std::size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->setText(_wp(texts[i]));
        buttonsControl[i]->setText(keyCodeName(configuration.controls[i]).c_str());
    }

    buttonDefaultControls->setText(_wp("Default"));
    buttonDefaultControls->setToolTipText(_wp("Default control settings"));

    buttonSettings->setText(_wp("Settings"));
    buttonSettings->setToolTipText(_wp("Back to game settings"));

    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));
}

void ControlSettingsScreen::terminate()
{
    for (std::size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->remove();
        buttonsControl[i]->remove();
    }

    buttonDefaultControls->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void ControlSettingsScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    for (std::size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                             configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                             configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                             configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE));

        buttonsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                               configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                               configuration.resolution.Width - 2 * SPACE,
                                                               configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE));
    }

    buttonDefaultControls->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
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

std::vector<gui::IGUIElement *> ControlSettingsScreen::getSelectableElements()
{
    std::vector<gui::IGUIElement *> selectableElements;
    for (auto &buttonControl : buttonsControl)
        selectableElements.push_back(buttonControl);


    selectableElements.push_back(buttonDefaultControls);
    selectableElements.push_back(buttonSettings);
    selectableElements.push_back(buttonQuit);

    return selectableElements;
}

void ControlSettingsScreen::setVisible(bool visible)
{
    textScreenSize->setVisible(visible);
    for (auto &buttonControl : buttonsControl)
        buttonControl->setVisible(visible);
    buttonDefaultControls->setVisible(visible);
    buttonSettings->setVisible(visible);
    buttonQuit->setVisible(visible);
}
