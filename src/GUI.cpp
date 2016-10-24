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
{}

GUI::~GUI()
{
    terminate();
}

GUIState GUI::getCurrentScreenIndex() const
{
    return currentScreenIndex;
}

bool addScreen(std::unique_ptr<IGUIScreen> screen, unsigned index)
{
    if (screens.find(index) != screens.end())
        return false;

    screens[index] = std::move(screen);
    return true;
}

bool removeScreen(unsigned screenIndex)
{
    if (screens.find(index) != screens.end())
        return false;

    screens.erase(screenIndex);
    return true;
}

IGUIScreen &getcurrentScreen()
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
}

void GUI::reload()
{
    getCurrentScreen().reload();
}

void GUI::updateSelection()
{
    getCurrentScreen().reload();

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
        if (auto element = selectableElements[i].lock()) {
            if (guiEnvironment.getFocus() == element.get())
                selectedElement = i;
        } else {
            return;
        }
    updateSelection();
}

void GUI::selectElement(size_t index)
{
    if (auto element = selectableElements[i].lock()) {
        guiEnvironment.setFocus(selectableElements[index].get());
        selectedElement = index;
        updateSelection();
    }
}

void GUI::selectNextElement()
{
    if (selectedElement < selectableElements.size() - 1)
        selectedElement++;
    else
        selectedElement = 0;

    if (auto element = selectableElements[selectedElement].lock()) {
        guiEnvironment.setFocus(selectableElements[selectedElement].get());
        updateSelection();
    }
}

void GUI::selectPreviousElement()
{
    if (selectedElement > 0)
        selectedElement--;
    else
        selectedElement = selectableElements.size() - 1;

    if (auto element = selectableElements[selectedElement].lock()) {
        guiEnvironment.setFocus(selectableElements[selectedElement].get());
        updateSelection();
    }
}

void GUI::terminate()
{
    selectableElements.clear();
    guiEnvironment.setFocus(0);
    selectedElement = 0;

    getCurrentScreen().terminate();
    currentScreenIndex = TERMINATED;
    getCurrentScreen().initialize(buttonWidth, buttonHeight);
}

void GUI::setVisible(bool visible)
{
    getCurrentScreen().setVisible(visible);
}

void GUI::resizeGUI()
{
    //changing gui position and size when window is resizing
    recalculateButtonProportions();
    getCurrentScreen().resize(buttonWidth, buttonHeight);
}
