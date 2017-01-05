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

#ifndef IGUISCREEN_H
#define IGUISCREEN_H

#include <vector>
#include <irrlicht.h>
#include "Config.hpp"

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

    void setCustomButtonSkin(gui::IGUIButton &button, bool isCSS = false) // CSS = ControlSettingsScreen :D
    {
        //use the sprite bank
        gui::IGUISpriteBank *spriteBank;
        if (!isCSS) spriteBank = guiEnvironment.getSpriteBank("SpritesForRegularButtons");
        else spriteBank = guiEnvironment.getSpriteBank("SpritesForControlButtons");
        button.setSpriteBank(spriteBank);
        button.setSprite(gui::EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
        button.setSprite(gui::EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
        button.setSprite(gui::EGBS_BUTTON_DOWN, 2, video::SColor(255, 255, 255, 255), false);
        button.setSprite(gui::EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
        button.setUseAlphaChannel(true);
        button.setDrawBorder(0);
    }
protected:
    const ConfigData &configuration;
    gui::IGUIEnvironment &guiEnvironment;

    static constexpr s32 SPACE = 10;
};

#endif // IGUISCREEN_H
