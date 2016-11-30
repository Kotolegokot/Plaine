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

#ifndef CONTROLSETTINGSSCREEN_H
#define CONTROLSETTINGSSCREEN_H

#include <array>
#include <irrlicht.h>
#include "interfaces/IGUIScreen.h"
#include "Config.h"
#include "util/i18n.h"

using namespace irr;

class ControlSettingsScreen : public IGUIScreen
{
public:
    ControlSettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~ControlSettingsScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textScreenSize;
    std::array<gui::IGUIStaticText *, CONTROLS_COUNT> textsControl;
    std::array<gui::IGUIButton *, CONTROLS_COUNT> buttonsControl;
    gui::IGUIButton *buttonDefaultControls;
    gui::IGUIButton *buttonSettings;
    gui::IGUIButton *buttonQuit;
};

#endif // CONTROLSETTINGSSCREEN_H
