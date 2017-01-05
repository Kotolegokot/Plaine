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

#ifndef HUDSCREEN_H
#define HUDSCREEN_H

#include <irrlicht.h>
#include "interfaces/IGUIScreen.hpp"
#include "Config.hpp"
#include "util/i18n.hpp"
#include "gui/GUIID.hpp"

using namespace irr;

class HUDScreen : public IGUIScreen
{
    bool infoVisible = true;
    bool allVisible = true;
    bool initialized = false;

public:
    HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~HUDScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    void setInfoVisible(bool visible);
    bool getInfoVisible() const;

    gui::IGUIStaticText *textCameraPosition;
    gui::IGUIStaticText *textObstaclesCount;
    gui::IGUIStaticText *textFPS;
    gui::IGUIStaticText *textVelocity;
    gui::IGUIStaticText *textAngle;
    gui::IGUIStaticText *textScore;
};

#endif // HUDSCREEN_H
