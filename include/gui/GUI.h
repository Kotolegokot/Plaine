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

#ifndef GUI_H
#define GUI_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <IGUIStaticText.h>
#include "interfaces/IGUIScreen.h"
#include "gui/screens/ControlSettingsScreen.h"
#include "gui/screens/HUDScreen.h"
#include "gui/screens/MainMenuScreen.h"
#include "gui/screens/PauseMenuScreen.h"
#include "gui/screens/SettingsScreen.h"
#include "gui/screens/GameOverScreen.h"
#include "gui/screens/ScoreboardScreen.h"

struct Screen {
    enum { TERMINATED, MAIN_MENU, SETTINGS, CONTROL_SETTINGS, PAUSE_MENU, HUD, GAME_OVER, SCOREBOARD };
};

class GUI
{
public:
    GUI(ConfigData &configuration,  gui::IGUIEnvironment &guiEnvironment);
    ~GUI();

    bool addScreen(std::unique_ptr<IGUIScreen> screen, unsigned index);
    bool removeScreen(unsigned screenIndex);
    unsigned getCurrentScreenIndex() const;
    IGUIScreen &getCurrentScreen();

    ControlSettingsScreen &getCurrentScreenAsControlSettings()
    {
        return static_cast<ControlSettingsScreen &>(getCurrentScreen());
    }

    HUDScreen &getCurrentScreenAsHUD()
    {
        return static_cast<HUDScreen &>(getCurrentScreen());
    }

    MainMenuScreen &getCurrentScreenAsMainMenu()
    {
        return static_cast<MainMenuScreen &>(getCurrentScreen());
    }

    PauseMenuScreen &getCurrentScreenAsPauseMenu()
    {
        return static_cast<PauseMenuScreen &>(getCurrentScreen());
    }

    SettingsScreen &getCurrentScreenAsSettings()
    {
        return static_cast<SettingsScreen &>(getCurrentScreen());
    }

    GameOverScreen &getCurrentScreenAsGameOver()
    {
        return static_cast<GameOverScreen &>(getCurrentScreen());
    }

    ScoreboardScreen &getCurrentScreenAsScoreboard()
    {
        return static_cast<ScoreboardScreen &>(getCurrentScreen());
    }

    void selectElement(std::size_t num);
    void selectNextElement();
    void selectPreviousElement();
    void selectWithTab();

    void initialize(unsigned screenIndex);
    void reload();
    void terminate();
    void resize();
    void setVisible(bool visible);
private:
    unsigned currentScreenIndex = 0;
    std::unordered_map<unsigned, std::unique_ptr<IGUIScreen>> screens;

    std::vector<gui::IGUIElement *> selectableElements;
    std::size_t selectedElement = 0;

    s32 buttonWidth, buttonHeight;

    ConfigData &configuration;
    gui::IGUIEnvironment &guiEnvironment;

    void updateSelection();
    void recalculateButtonProportions();
};

#endif // GUI_H
