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

#ifndef GUI_H
#define GUI_H

#include <vector>
#include <array>
#include "Config.h"
#include "util.h"

enum GUIState { MAIN_MENU, PAUSE_MENU, HUD, SETTINGS, CONTROL_SETTINGS, TERMINATED };

class GUI
{
public:
    GUI(ConfigData &data, gui::IGUIEnvironment &guiEnvironment, video::IVideoDriver *driver);
    ~GUI();
    ConfigData &configuration;
    int buttonWidth, buttonHeight;

    // GUI
    video::IVideoDriver *driver = nullptr;
    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonToggleFullscreen = nullptr;
    gui::IGUIButton *buttonControlSettings = nullptr;
    gui::IGUIButton *buttonDefaultControls = nullptr;
    std::array<gui::IGUIButton *, CONTROLS_COUNT> buttonsControl;
    gui::IGUIButton *buttonResume = nullptr;
    gui::IGUIButton *buttonMenu = nullptr;
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIComboBox *comboBoxResolution = nullptr;
    gui::IGUIComboBox *comboBoxLanguage = nullptr;
    gui::IGUIStaticText *textScreenSize = nullptr;
    gui::IGUIStaticText *textCubeCount = nullptr;
    gui::IGUIStaticText *textFPS = nullptr;
    gui::IGUIStaticText *textVelocity = nullptr;
    gui::IGUIStaticText *textPoints = nullptr;
    gui::IGUIStaticText *textResolution = nullptr;
    gui::IGUIStaticText *textRenderDistance = nullptr;
    gui::IGUIStaticText *textLanguage = nullptr;
    gui::IGUIStaticText *textCameraPos = nullptr;
    std::array<gui::IGUIStaticText *, CONTROLS_COUNT> textsControl;
    gui::IGUICheckBox *checkBoxVSync = nullptr;
    gui::IGUICheckBox *checkBoxStencilBuffer = nullptr;
    gui::IGUISpinBox *spinBoxRenderDistance = nullptr;

    void initialize(GUIState state);
    void reload();
    void terminate();
    void resizeGUI();
    void setVisible(bool visible);
    void selectElement(int num);
    void selectNextElement();
    void selectPreviousElement();
    void selectWithTab();

    void setHUDInfoVisible(bool visible);
    bool getHUDInfoVisible() const;

    void setCustomButton(char src[], gui::IGUIButton *button);

    GUIState getState() const;
private:
    GUIState state = TERMINATED;
    std::vector<gui::IGUIElement*> selectableElements;
    size_t selectedElement;
    const int SPACE = 10;

    gui::IGUIEnvironment &guiEnvironment;

    void updateSelection();
    void recalculateButtonProportions();

    void initializeMenuGUI();
    void initializeInGameGUI();
    void initializeHUD();
    void initializeSettingsGUI();
    void initializeControlSettingsGUI();

    void terminateMenuGUI();
    void terminateSettingsGUI();
    void terminateHUD();
    void terminateControlSettingsGUI();
    void terminateInGameGUI();

    bool HUDInfoVisible = true;
};

#endif // GUI_H
