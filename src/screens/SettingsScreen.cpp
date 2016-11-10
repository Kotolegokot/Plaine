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

#include "screens/SettingsScreen.h"

SettingsScreen::SettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

SettingsScreen::~SettingsScreen()
{
    terminate();
}

void SettingsScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(0, 0, 0, 0));
    textLanguage = guiEnvironment.addStaticText(_wp("Language:"), core::rect<s32>(0, 0, 0, 0));

    comboBoxLanguage = guiEnvironment.addComboBox(core::rect<s32>(0, 0, 0, 0));
    comboBoxLanguage->setID(ID_COMBOBOX_LANGUAGE);

    textResolution = guiEnvironment.addStaticText(_wp("Resolution:"), core::rect<s32>(0, 0, 0, 0));

    comboBoxResolution = guiEnvironment.addComboBox(core::rect<s32>(0, 0, 0, 0));
    comboBoxResolution->setID(ID_COMBOBOX_RESOLUTION);

    textRenderDistance = guiEnvironment.addStaticText(_wp("Render distance:"), core::rect<s32>(0, 0, 0, 0));

    spinBoxRenderDistance = guiEnvironment.addSpinBox(_wp("Render distance"), core::rect<s32>(0, 0, 0, 0));
    spinBoxRenderDistance->setID(ID_SPINBOX_RENDER_DISTANCE);

    checkBoxVSync = guiEnvironment.addCheckBox(configuration.vsync, core::rect<s32>(0, 0, 0, 0));
    checkBoxVSync->setID(ID_CHECKBOX_VSYNC);

    checkBoxStencilBuffer = guiEnvironment.addCheckBox(configuration.stencilBuffer, core::rect<s32>(0, 0, 0, 0));
    checkBoxStencilBuffer->setID(ID_CHECKBOX_STENCILBUFFER);

    buttonToggleFullscreen = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonToggleFullscreen->setID(ID_BUTTON_TOGGLE_FULLSCREEN);
    setCustomButton(buttonToggleFullscreen);

    buttonControlSettings = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonControlSettings->setID(ID_BUTTON_CONTROL_SETTINGS);
    setCustomButton(buttonControlSettings);

    buttonMenu = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonMenu->setID(ID_BUTTON_MENU);
    setCustomButton(buttonMenu);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButton(buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
}

void SettingsScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    comboBoxLanguage->addItem(_wp("System language"), 0);
    comboBoxLanguage->addItem(L"English", 1);
    comboBoxLanguage->addItem(L"Русский", 2);
    comboBoxLanguage->addItem(L"Qırımtatarca", 3);
    comboBoxLanguage->addItem(L"Къырымтатарджа", 4);
    if (configuration.language.empty())
        comboBoxLanguage->setSelected(0);
    else if (configuration.language == "en")
        comboBoxLanguage->setSelected(1);
    else if (configuration.language == "ru")
        comboBoxLanguage->setSelected(2);
    else if (configuration.language == "crh")
        comboBoxLanguage->setSelected(3);
    else if (configuration.language == "crh@cyrillic")
        comboBoxLanguage->setSelected(4);

    if (configuration.fullscreen)
    {
        core::stringw scrs;
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        scrs += _w(" (Fullscreen)");
        comboBoxResolution->addItem(scrs.c_str(), 0);
        comboBoxResolution->setSelected(0);
        comboBoxResolution->setEnabled(false);

    } else {
        comboBoxResolution->addItem(L"640x480", 0);
        comboBoxResolution->addItem(L"1240x720", 1);
        if (configuration.resizable) {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        } else if (configuration.resolution == core::dimension2d<u32>(640, 480)) {
            comboBoxResolution->setSelected(0);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        } else if (configuration.resolution == core::dimension2d<u32>(1240, 720)) {
            comboBoxResolution->setSelected(1);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        } else {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
    }

    spinBoxRenderDistance->setRange(500, 5000);
    spinBoxRenderDistance->setStepSize(100);
    spinBoxRenderDistance->setValue(configuration.renderDistance);

    checkBoxVSync->setText(_wp("VSync"));

    checkBoxStencilBuffer->setText(_wp("Stencil buffer"));

    if (configuration.fullscreen) {
        buttonToggleFullscreen->setText(_wp("Windowed"));
        buttonToggleFullscreen->setToolTipText(_wp("To windowed mode"));
    } else {
        buttonToggleFullscreen->setText(_wp("Fullscreen"));
        buttonToggleFullscreen->setToolTipText(_wp("To fullscreen mode"));
    }

    buttonControlSettings->setText(_wp("Controls"));
    buttonControlSettings->setToolTipText(_wp("Edit control settings"));

    buttonMenu->setText(_wp("Back"));
    buttonMenu->setToolTipText(_wp("Exit to main menu"));

    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));
}

void SettingsScreen::terminate()
{
    textScreenSize->remove();
    textLanguage->remove();
    textResolution->remove();
    textRenderDistance->remove();
    comboBoxLanguage->remove();
    comboBoxResolution->remove();
    spinBoxRenderDistance->remove();
    checkBoxVSync->remove();
    checkBoxStencilBuffer->remove();
    buttonToggleFullscreen->remove();
    buttonControlSettings->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void SettingsScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    textLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                      configuration.resolution.Height - 9 * buttonHeight - 9 * space,
                                                      configuration.resolution.Width - buttonWidth - 2 * space,
                                                      configuration.resolution.Height - 8 * buttonHeight - 9 * space));

    comboBoxLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                          configuration.resolution.Height - 9 * buttonHeight - 9 * space,
                                                          configuration.resolution.Width - 2 * space,
                                                          configuration.resolution.Height - 8 * buttonHeight - 9 * space));

    textResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                        configuration.resolution.Height - 8 * buttonHeight - 8 * space,
                                                        configuration.resolution.Width - buttonWidth - 2 * space,
                                                        configuration.resolution.Height - 7 * buttonHeight - 8 * space));

    comboBoxResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 8 * buttonHeight - 8 * space,
                                                            configuration.resolution.Width - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 8 * space));

    textRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 7 * space,
                                                            configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 6 * buttonHeight - 7*space));

    spinBoxRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 7 * space,
                                                            configuration.resolution.Width - 2 * space,
                                                            configuration.resolution.Height - 6 * buttonHeight - 7 * space));

    checkBoxVSync->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                       configuration.resolution.Height - 6 * buttonHeight - 6 * space,
                                                       configuration.resolution.Width - 2 * space,
                                                       configuration.resolution.Height - 5 * buttonHeight - 6 * space));

    checkBoxStencilBuffer->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                               configuration.resolution.Height - 5 * buttonHeight - 5 * space,
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - 4 * buttonHeight - 5 * space));

    buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                                configuration.resolution.Height - 4 * buttonHeight - 4 * space,
                                                                configuration.resolution.Width - 2 * space,
                                                                configuration.resolution.Height - 3 * buttonHeight - 4 * space));

    buttonControlSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                               configuration.resolution.Height - 3 * buttonHeight - 3 * space,
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - 2 * buttonHeight - 3 * space));

    buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - 2 * buttonHeight - 2 * space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - 2 * space));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - space));

    //if window size has changed, its resolution is not standard
    comboBoxResolution->removeItem(2);
    core::stringw scrs;
    scrs += configuration.resolution.Width;
    scrs += "x";
    scrs += configuration.resolution.Height;
    scrs += _wp(" (Custom)");
    comboBoxResolution->addItem(scrs.c_str(), 2);
    comboBoxResolution->setSelected(2);
}

std::vector<gui::IGUIElement *> SettingsScreen::getSelectableElements()
{
    return { checkBoxVSync, checkBoxStencilBuffer, buttonToggleFullscreen,
        buttonControlSettings, buttonMenu, buttonQuit };
}

void SettingsScreen::setVisible(bool visible)
{
    textScreenSize->setVisible(visible);
    textLanguage->setVisible(visible);
    textResolution->setVisible(visible);
    textRenderDistance->setVisible(visible);
    comboBoxLanguage->setVisible(visible);
    comboBoxResolution->setVisible(visible);
    spinBoxRenderDistance->setVisible(visible);
    checkBoxVSync->setVisible(visible);
    checkBoxStencilBuffer->setVisible(visible);
    buttonToggleFullscreen->setVisible(visible);
    buttonControlSettings->setVisible(visible);
    buttonMenu->setVisible(visible);
    buttonQuit->setVisible(visible);
}
