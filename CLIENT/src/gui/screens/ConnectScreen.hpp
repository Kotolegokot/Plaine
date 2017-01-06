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

#pragma once

#include <irrlicht.h>
#include "interfaces/IGUIScreen.hpp"
#include "Config.hpp"
#include "util/i18n.hpp"
#include "gui/GUIID.hpp"

using namespace irr;

class ConnectScreen : public IGUIScreen
{
   bool initialized = false;

   core::rect<s32> left_column(int n, s32 button_width, s32 button_height) const;
   core::rect<s32> right_column(int n, s32 button_width, s32 button_height) const;

public:
   ConnectScreen(const ConfigData &configuration, gui::IGUIEnvironment &gui_env);
   ~ConnectScreen();

   virtual void initialize(s32 button_width, s32 button_height) override;
   virtual void reload(s32 button_width, s32 button_height) override;
   virtual void terminate() override;
   virtual void resize(s32 button_width, s32 button_height) override;
   virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
   virtual void setVisible(bool visible) override;

   gui::IGUIStaticText *text_nick;
   gui::IGUIStaticText *text_host;
   gui::IGUIStaticText *text_port;
   gui::IGUIEditBox *editbox_nick;
   gui::IGUIEditBox *editbox_host;
   gui::IGUISpinBox *spinbox_port;
   gui::IGUIButton *button_connect;
   gui::IGUIButton *button_menu;
   gui::IGUIButton *button_quit;
};
