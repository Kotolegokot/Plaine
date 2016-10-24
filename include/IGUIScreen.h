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

#ifndef IGUISCREEN_H
#define IGUISCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <irrlicht.h>
#include "Config.h"

using namespace irr;

class IGUIScreen
{
public:
    IGUIScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
        configuration(configuration), guiEnvironment(guiEnvironment) {}
    virtual ~IGUIScreen() {}

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void terminate() = 0;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() = 0;
    virtual void setVisible(bool visible) = 0;

protected:
    const ConfigData &configuration;
    gui::IGUIEnvironment &guiEnvironment;

    static constexpr s32 space = 10;
};

#endif // IGUISCREEN_H
