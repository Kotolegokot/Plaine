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

#ifndef PAUSEMENUSCREEN_H
#define PAUSEMENUSCREEN_H

#include <irrlicht.h>
#include "interfaces/IGUIScreen.h"
#include "Config.h"
#include "util/i18n.h"

using namespace irr;

class PauseMenuScreen : public IGUIScreen
{
public:
    PauseMenuScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~PauseMenuScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    gui::IGUIStaticText *textScreenSize;
    gui::IGUIButton *buttonResume;
    gui::IGUIButton *buttonMenu;
    gui::IGUIButton *buttonQuit;
};

#endif // PAUSEMENUSCREEN_H
