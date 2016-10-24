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

#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class SettingsScreen : public IGUIScreen
{
public:
    SettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~SettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textScreenSize;
    gui::IGUIStaticText *textLanguage;
    gui::IGUIStaticText *textResolution;
    gui::IGUIStaticText *textRenderDistance;
    gui::IGUIComboBox *comboBoxLanguage;
    gui::IGUIComboBox *comboBoxResolution;
    gui::IGUISpinBox *spinBoxRenderDistance;
    gui::IGUICheckBox *checkBoxVSync;
    gui::IGUICheckBox *checkBoxStencilBuffer;
    gui::IGUIButton *buttonToggleFullscreen;
    gui::IGUIButton *buttonControlSettings;
    gui::IGUIButton *buttonMenu;
    gui::IGUIButton *buttonQuit;
};

#endif // SETTINGSSCREEN_H
