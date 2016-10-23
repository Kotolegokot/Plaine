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

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <cstdlib>
#include <irrlicht.h>
#include <libintl.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "Randomizer.h"

#ifndef _WIN32
#include <iconv.h>
#else
#include <Windows.h>
#endif

using namespace irr;

#define _(string) gettext(string)
#define _w(string) utf8_to_irrwide(gettext(string))
#define _wp(string) utf8_to_wide(gettext(string)).c_str()

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
core::stringw utf8_to_irrwide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);
core::stringw keyCodeName(const EKEY_CODE &keyCode);
void setLanguage(std::string language, bool replace);
video::SColor iridescentColor(const u32 &currentTime);

// number of GUI elements that have an id
constexpr size_t GUI_IDS_COUNT = 19;

// GUI IDs
// !! don't forget to change GUI_IDS_COUNT if you change this enumeration !!
enum GUI_ID {
    ID_BUTTON_QUIT,
    ID_BUTTON_START,
    ID_BUTTON_SETTINGS,
    ID_BUTTON_MENU,
    ID_BUTTON_TOGGLE_FULLSCREEN,
    ID_BUTTON_RESUME,
    ID_BUTTON_CONTROL_SETTINGS,
    ID_BUTTON_CONTROL_UP,
    ID_BUTTON_CONTROL_LEFT,
    ID_BUTTON_CONTROL_DOWN,
    ID_BUTTON_CONTROL_RIGHT,
    ID_BUTTON_CONTROL_CW_ROLL,
    ID_BUTTON_CONTROL_CCW_ROLL,
    ID_BUTTON_DEFAULT_CONTROLS,
    ID_COMBOBOX_RESOLUTION,
    ID_COMBOBOX_LANGUAGE,
    ID_CHECKBOX_VSYNC,
    ID_CHECKBOX_STENCILBUFFER,
    ID_SPINBOX_RENDER_DISTANCE };


constexpr f32 COLOR_CHANGE_INTERVAL = 3000.0f;

#endif // UTIL_H_INCLUDED
