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

class TerminatedScreen : public IGUIScreen
{
public:
    TerminatedScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
        IGUIScreen(configuration, guiEnvironment) {}
    virtual ~TerminatedScreen() {}

    virtual void initialize(s32, s32) override {}
    virtual void reload(s32, s32) override {}
    virtual void terminate() override {}
    virtual void resize(s32, s32) override {}
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override { return {}; }
    virtual void setVisible(bool) override {}
};

GUI::GUI(ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    configuration(configuration), guiEnvironment(guiEnvironment)
{
    addScreen(std::make_unique<TerminatedScreen>(configuration, guiEnvironment), Screen::TERMINATED);
    addScreen(std::make_unique<MainMenuScreen>(configuration, guiEnvironment), Screen::MAIN_MENU);
    addScreen(std::make_unique<SettingsScreen>(configuration, guiEnvironment), Screen::SETTINGS);
    addScreen(std::make_unique<ControlSettingsScreen>(configuration, guiEnvironment), Screen::CONTROL_SETTINGS);
    addScreen(std::make_unique<PauseMenuScreen>(configuration, guiEnvironment), Screen::PAUSE_MENU);
    addScreen(std::make_unique<HUDScreen>(configuration, guiEnvironment), Screen::HUD);
}

GUI::~GUI()
{
    terminate();
}

unsigned GUI::getCurrentScreenIndex() const
{
    return currentScreenIndex;
}

bool GUI::addScreen(std::unique_ptr<IGUIScreen> screen, unsigned index)
{
    if (screens.find(index) != screens.end())
        return false;

    screens[index] = std::move(screen);
    return true;
}

bool GUI::removeScreen(unsigned screenIndex)
{
    if (screens.find(screenIndex) != screens.end())
        return false;

    screens.erase(screenIndex);
    return true;
}

IGUIScreen &GUI::getCurrentScreen()
{
    return *screens[currentScreenIndex];
}

void GUI::recalculateButtonProportions()
{
    // button width calculated from window width
    buttonWidth = configuration.resolution.Width / 6;

    // minimal button width
    if (buttonWidth < 220)
        buttonWidth = 220;

    // height calculated from width
    buttonHeight = buttonWidth * 1/8;
}

void GUI::initialize(unsigned screenIndex)
{
    recalculateButtonProportions();
    getCurrentScreen().terminate();
    currentScreenIndex = screenIndex;
    getCurrentScreen().initialize(buttonWidth, buttonHeight);
    selectableElements = getCurrentScreen().getSelectableElements();
}

void GUI::reload()
{
    getCurrentScreen().reload(buttonWidth, buttonHeight);
}

void GUI::updateSelection()
{
    getCurrentScreen().reload(buttonWidth, buttonHeight);

    if (guiEnvironment.getFocus()->getType() != gui::EGUIET_EDIT_BOX &&
        guiEnvironment.getFocus()->getType() != gui::EGUIET_COMBO_BOX)
    {
        core::stringw str = "[";
        str += guiEnvironment.getFocus()->getText();
        str += "]";
        guiEnvironment.getFocus()->setText(str.c_str());
    }
}

void GUI::selectWithTab()
{
    for (size_t i = 0; i < selectableElements.size(); i++)
        if (guiEnvironment.getFocus() == selectableElements[i]) {
            selectedElement = i;
            break;
        }
    updateSelection();
}

void GUI::selectElement(size_t index)
{
    guiEnvironment.setFocus(selectableElements[index]);
    selectedElement = index;
    updateSelection();
}

void GUI::selectNextElement()
{
    if (selectedElement < selectableElements.size() - 1)
        selectedElement++;
    else
        selectedElement = 0;

    guiEnvironment.setFocus(selectableElements[selectedElement]);
    updateSelection();
}

void GUI::selectPreviousElement()
{
    if (selectedElement > 0)
        selectedElement--;
    else
        selectedElement = selectableElements.size() - 1;

    guiEnvironment.setFocus(selectableElements[selectedElement]);
    updateSelection();
}

void GUI::terminate()
{
    initialize(Screen::TERMINATED);

    guiEnvironment.setFocus(0);
    selectedElement = 0;
}

void GUI::setVisible(bool visible)
{
    getCurrentScreen().setVisible(visible);
}

void GUI::resize()
{
    //changing gui position and size when window is resizing
    recalculateButtonProportions();
    getCurrentScreen().resize(buttonWidth, buttonHeight);
}
