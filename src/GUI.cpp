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

#include "GUI.h"

using namespace irr;

GUI::GUI(ConfigData &data, gui::IGUIEnvironment &guiEnvironment) :
    configuration(data), guiEnvironment(guiEnvironment)
{
    textsControl.fill(nullptr);
    buttonsControl.fill(nullptr);
}

GUI::~GUI()
{
    terminate();
}

GUIState GUI::getState() const
{
    return state;
}

void GUI::recalculateButtonProportions()
{
    //width for buttons calculates from width of window
    buttonWidth = configuration.resolution.Width / 6;
    //minimal width for buttons
    if (buttonWidth < 220)
        buttonWidth = 220;
    //height calculated from width
    buttonHeight = buttonWidth * 1/8;
}

void GUI::initialize(GUIState state)
{
    // terminate first
    terminate();

    // choose specific method for initializing GUI
    switch (state)
    {
    case(MAIN_MENU):
        initializeMenuGUI();
        break;
    case(PAUSE_MENU):
        initializeInGameGUI();
        break;
    case(HUD):
        initializeHUD();
        break;
    case(SETTINGS):
        initializeSettingsGUI();
        break;
     case(CONTROL_SETTINGS):
        initializeControlSettingsGUI();
        break;
    case(TERMINATED):
        break;
    }
    this->state = state;
}

void GUI::reload()
{
    initialize(state);
}

void GUI::initializeMenuGUI()
{
    recalculateButtonProportions();
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_START, _wp("Start"), _wp("Start game"));
    buttonSettings = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_SETTINGS, _wp("Settings"), _wp("Game settings"));
    buttonQuit = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    selectableElements.push_back(buttonStart);
    selectableElements.push_back(buttonSettings);
    selectableElements.push_back(buttonQuit);
}

void GUI::terminateMenuGUI()
{
    buttonStart->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void GUI::initializeInGameGUI()
{
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonResume = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_RESUME, _wp("Resume"), _wp("Resume Game"));
    buttonMenu = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Menu"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    selectableElements.push_back(buttonResume);
    selectableElements.push_back(buttonMenu);
    selectableElements.push_back(buttonQuit);
}

void GUI::terminateInGameGUI()
{
    textScreenSize->remove();
    buttonResume->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void GUI::initializeHUD()
{
    recalculateButtonProportions();
    textCameraPos = guiEnvironment.addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    textCameraPos->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textCubeCount = guiEnvironment.addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24), false);
    textCubeCount->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textFPS = guiEnvironment.addStaticText(L"FPS", core::rect<s32>(10, 10 + 24 + 24, 400, 30 + 24 + 24), false);
    textFPS->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textVelocity = guiEnvironment.addStaticText(L"Velocity", core::rect<s32>(10, 10 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24), false);
    textVelocity->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textPoints = guiEnvironment.addStaticText(L"Points", core::rect<s32>(10, 10 + 24 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24 + 24), false);
    textPoints->setBackgroundColor(video::SColor(120, 255, 255, 255));
}

void GUI::terminateHUD()
{
    textCameraPos->remove();
    textCubeCount->remove();
    textFPS->remove();
    textVelocity->remove();
    textPoints->remove();
}

void GUI::initializeSettingsGUI()
{
    recalculateButtonProportions();
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    textLanguage = guiEnvironment.addStaticText(_wp("Language:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE), false);
    comboBoxLanguage = guiEnvironment.addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE), 0, ID_COMBOBOX_LANGUAGE);

    comboBoxLanguage->addItem(_wp("System language"), 0);
    comboBoxLanguage->addItem(L"English", 1);
    comboBoxLanguage->addItem(L"Русский", 2);
    comboBoxLanguage->addItem(L"Qırımtatarca", 3);
    comboBoxLanguage->addItem(L"Къырымтатарджа", 4);

    if (configuration.language == "")
        comboBoxLanguage->setSelected(0);
    else if (configuration.language == "en")
        comboBoxLanguage->setSelected(1);
    else if (configuration.language == "ru")
        comboBoxLanguage->setSelected(2);
    else if (configuration.language == "crh")
        comboBoxLanguage->setSelected(3);
    else if (configuration.language == "crh@cyrillic")
        comboBoxLanguage->setSelected(4);
    textResolution = guiEnvironment.addStaticText(_wp("Resolution:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE), false);
    comboBoxResolution = guiEnvironment.addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE), 0, ID_COMBOBOX_RESOLUTION);
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

    }
    else {
        comboBoxResolution->addItem(L"640x480", 0);
        comboBoxResolution->addItem(L"1240x720", 1);
        if (configuration.resizable)
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(640, 480))
        {
            comboBoxResolution->setSelected(0);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(1240, 720))
        {
            comboBoxResolution->setSelected(1);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        }
        else
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
    }
    textRenderDistance = guiEnvironment.addStaticText(_wp("Render distance:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE,
                                                                                                 configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE), false);
    spinBoxRenderDistance = guiEnvironment.addSpinBox(_wp("Render distance"), core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE,
                                                                                                configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE), true, 0, ID_SPINBOX_RENDER_DISTANCE);
    spinBoxRenderDistance->setRange(500, 5000);
    spinBoxRenderDistance->setStepSize(100);
    spinBoxRenderDistance->setValue(configuration.renderDistance);
    checkBoxVSync = guiEnvironment.addCheckBox(configuration.vsync, core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE),
        0, ID_CHECKBOX_VSYNC, _wp("VSync"));
    checkBoxStencilBuffer = guiEnvironment.addCheckBox(configuration.stencilBuffer, core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE),
        0, ID_CHECKBOX_STENCILBUFFER, _wp("Stencil Buffer"));

    if (configuration.fullscreen)
        buttonToggleFullscreen = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE),
            0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Windowed"), _wp("To Windowed Mode"));
    else
        buttonToggleFullscreen = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE),
            0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Fullscreen"), _wp("To Fullscreen Mode"));
    buttonControlSettings = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_CONTROL_SETTINGS, _wp("Controls"), _wp("Edit control settings"));
    buttonMenu = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Back"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    selectableElements.push_back(checkBoxVSync);
    selectableElements.push_back(checkBoxStencilBuffer);
    selectableElements.push_back(buttonToggleFullscreen);
    selectableElements.push_back(buttonControlSettings);
    selectableElements.push_back(buttonMenu);
    selectableElements.push_back(buttonQuit);
}

void GUI::terminateSettingsGUI()
{
    textScreenSize->remove();
    textLanguage->remove();
    comboBoxLanguage->remove();
    textResolution->remove();
    comboBoxResolution->remove();
    textRenderDistance->remove();
    spinBoxRenderDistance->remove();
    checkBoxVSync->remove();
    checkBoxStencilBuffer->remove();
    buttonToggleFullscreen->remove();
    buttonControlSettings->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void GUI::initializeControlSettingsGUI()
{
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);

    static constexpr std::array<const char *, CONTROLS_COUNT> texts = { "Up: ", "Left: ", "Down: ", "Right: ", "CW roll", "CCW roll" };
    static constexpr std::array<u32, CONTROLS_COUNT> ids = { ID_BUTTON_CONTROL_UP, ID_BUTTON_CONTROL_LEFT,
                                                             ID_BUTTON_CONTROL_DOWN, ID_BUTTON_CONTROL_RIGHT,
                                                             ID_BUTTON_CONTROL_CW_ROLL, ID_BUTTON_CONTROL_CCW_ROLL };
    for (size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i] = guiEnvironment.addStaticText(_wp(texts[i]),
                                                       core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                                       configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                                       configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                                       configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE),
                                                       false);

        buttonsControl[i] = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                                     configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                                     configuration.resolution.Width - 2 * SPACE,
                                                                     configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE),
                                                     0, ids[i], keyCodeName(configuration.controls[i]).c_str(), _wp("Click to change"));
    }

    buttonDefaultControls = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_DEFAULT_CONTROLS, _wp("Default"), _wp("Default control settings"));
    buttonSettings = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_SETTINGS, _wp("Back"), _wp("Back to game settings"));
    buttonQuit = guiEnvironment.addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    state = CONTROL_SETTINGS;
    for (auto buttonControl : buttonsControl)
        selectableElements.push_back(buttonControl);
    selectableElements.push_back(buttonDefaultControls);
    selectableElements.push_back(buttonSettings);
    selectableElements.push_back(buttonQuit);
}

void GUI::terminateControlSettingsGUI()
{
    // TODO
    for (size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->remove();
        buttonsControl[i]->remove();
    }
    buttonDefaultControls->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void GUI::updateSelection()
{
    switch (state)
    {
    case(MAIN_MENU):
            buttonStart->setText(_wp("Start"));
            buttonSettings->setText(_wp("Settings"));
            buttonQuit->setText(_wp("Quit"));
            break;
    case(PAUSE_MENU):
            buttonResume->setText(_wp("Resume"));
            buttonMenu->setText(_wp("Menu"));
            buttonQuit->setText(_wp("Quit"));
            break;
        case(HUD):
            break;
    case(SETTINGS):
            checkBoxVSync->setText(_wp("VSync"));
            checkBoxStencilBuffer->setText(_wp("Stencil Buffer"));
            if (!configuration.fullscreen)
                buttonToggleFullscreen->setText(_wp("Fullscreen"));
            else
                buttonToggleFullscreen->setText(_wp("Windowed"));
            buttonControlSettings->setText(_wp("Controls"));
            buttonMenu->setText(_wp("Back"));
            buttonQuit->setText(_wp("Quit"));
            break;
     case(CONTROL_SETTINGS):
            buttonsControl[CONTROL::UP]->setText(keyCodeName(configuration.controls[CONTROL::UP]).c_str());
            buttonsControl[CONTROL::LEFT]->setText(keyCodeName(configuration.controls[CONTROL::LEFT]).c_str());
            buttonsControl[CONTROL::DOWN]->setText(keyCodeName(configuration.controls[CONTROL::DOWN]).c_str());
            buttonsControl[CONTROL::RIGHT]->setText(keyCodeName(configuration.controls[CONTROL::RIGHT]).c_str());
            buttonsControl[CONTROL::CCW_ROLL]->setText(keyCodeName(configuration.controls[CONTROL::CCW_ROLL]).c_str());
            buttonsControl[CONTROL::CW_ROLL]->setText(keyCodeName(configuration.controls[CONTROL::CW_ROLL]).c_str());
            buttonDefaultControls->setText(_wp("Default"));
            buttonSettings->setText(_wp("Back"));
            buttonQuit->setText(_wp("Quit"));
            break;
    case(TERMINATED):
        break;
    }
    if ((guiEnvironment.getFocus()->getType() != (gui::EGUIET_EDIT_BOX)) && (guiEnvironment.getFocus()->getType() != gui::EGUIET_COMBO_BOX))
    {
        core::stringw str = ">";
        str += guiEnvironment.getFocus()->getText();
        str += "<";
        guiEnvironment.getFocus()->setText(str.c_str());
    }
}


void GUI::selectWithTab()
{
    for (size_t i = 0; i < selectableElements.size(); i++)
        if (guiEnvironment.getFocus() == selectableElements.at(i))
            selectedElement = i;
    updateSelection();
}

void GUI::selectElement(int num)
{
    guiEnvironment.setFocus(selectableElements.at(num));
    selectedElement = num;
    updateSelection();
}

void GUI::selectNextElement()
{
    if (selectedElement < selectableElements.size() - 1)
        selectedElement++;
    else
        selectedElement = 0;
    guiEnvironment.setFocus(selectableElements.at(selectedElement));
    updateSelection();
}

void GUI::selectPreviousElement()
{
    if (selectedElement > 0)
        selectedElement--;
    else
        selectedElement = selectableElements.size() - 1;
    guiEnvironment.setFocus(selectableElements.at(selectedElement));
    updateSelection();
}

void GUI::terminate()
{
    //choose specific method for terminating GUI
    switch (state)
    {
    case(MAIN_MENU):
            terminateMenuGUI();
            break;
    case(PAUSE_MENU):
            terminateInGameGUI();
            break;
        case(HUD):
            terminateHUD();
            break;
    case(SETTINGS):
            terminateSettingsGUI();
            break;
     case(CONTROL_SETTINGS):
            terminateControlSettingsGUI();
            break;
    case(TERMINATED):
        break;
    }

    this->state = TERMINATED;
    selectableElements.clear();
    guiEnvironment.setFocus(0);
    selectedElement = 0;
}

void GUI::setVisible(bool visible)
{
    switch (state)
    {
    case (MAIN_MENU):
        {
            textScreenSize->setVisible(visible);
            buttonStart->setVisible(visible);
            buttonSettings->setVisible(visible);
            buttonQuit->setVisible(visible);
            break;
        }
    case (PAUSE_MENU):
        {
            buttonResume->setVisible(visible);
            buttonMenu->setVisible(visible);
            buttonQuit->setVisible(visible);
            textScreenSize->setVisible(visible);
            break;
        }
    case (HUD):
        {
            textCameraPos->setVisible(visible);
            textCubeCount->setVisible(visible);
            textFPS->setVisible(visible);
            textVelocity->setVisible(visible);
            textPoints->setVisible(visible);
            break;
        }
    case (SETTINGS):
        {
            textScreenSize->setVisible(visible);
            textLanguage->setVisible(visible);
            comboBoxLanguage->setVisible(visible);
            textResolution->setVisible(visible);
            comboBoxResolution->setVisible(visible);
            textRenderDistance->setVisible(visible);
            spinBoxRenderDistance->setVisible(visible);
            checkBoxVSync->setVisible(visible);
            checkBoxStencilBuffer->setVisible(visible);
            buttonToggleFullscreen->setVisible(visible);
            buttonControlSettings->setVisible(visible);
            buttonMenu->setVisible(visible);
            buttonQuit->setVisible(visible);
            break;
        }
    case (CONTROL_SETTINGS):
        {
            textScreenSize->setVisible(visible);
            for (size_t i = 0; i < CONTROLS_COUNT; i++) {
                textsControl[i]->setVisible(visible);
                buttonsControl[i]->setVisible(visible);
            }
            buttonDefaultControls->setVisible(visible);
            buttonSettings->setVisible(visible);
            buttonQuit->setVisible(visible);
            break;
        }
    case (TERMINATED):
        break;
    }
}

void GUI::resizeGUI()
{
    //changing gui position and size when window is resizing
    recalculateButtonProportions();
    switch (state)
    {
    case (MAIN_MENU):
        {
            buttonStart->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (PAUSE_MENU):
        {
            buttonResume->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (SETTINGS):
        {
            textLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE,
                        configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE));
            comboBoxLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE));
            textResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE,
                        configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE));
            comboBoxResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE));
            textRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE,
                        configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));
            spinBoxRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 7*buttonHeight - 7*SPACE,configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));
            checkBoxVSync->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            checkBoxStencilBuffer->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            buttonControlSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE,
                        configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            //if window size has changed, its resolution is not standard
            comboBoxResolution->removeItem(2);
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _wp(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
            break;
        }
    case (CONTROL_SETTINGS):
        {
            for (size_t i = 0; i < CONTROLS_COUNT; i++) {
                textsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                                     configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                                     configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                                     configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE));

                buttonsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth / 2 - 2 * SPACE,
                                                                       configuration.resolution.Height - (9 - i) * (buttonHeight + SPACE),
                                                                       configuration.resolution.Width - 2 * SPACE,
                                                                       configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * SPACE));
            }
            buttonDefaultControls->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (HUD):
        break;
    case (TERMINATED):
        break;
    }
}
