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

#include <array>
#include <string>
#include <cstdlib>
#include <irrlicht.h>
#include <libintl.h>
#include <btBulletDynamicsCommon.h>
#include "Randomizer.h"
#include "util/Vector3.h"
#include "util/Array3.h"
#include "util/Cuboid.h"

#ifndef _WIN32
#include <iconv.h>
#else
#include <Windows.h>
#endif

using namespace irr;

#define _(string) gettext(string)
#define _w(string) utf8_to_irrwide(gettext(string))
#define _wp(string) utf8_to_wide(gettext(string)).c_str()

constexpr btScalar MASS_COEFFICIENT = 0.000002;

template<typename T>
constexpr T PI = T(3.1415926535897932385);

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
core::stringw utf8_to_irrwide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);
core::stringw keyCodeName(const EKEY_CODE &keyCode);
void setLanguage(std::string language, bool replace);
video::SColor iridescentColor(const u32 &currentTime);

template <typename Num>
btVector3 irrlicht2bullet(const core::vector3d<Num> &irrVector)
{
    return btVector3(irrVector.X, irrVector.Y, irrVector.Z);
}

core::vector3df bullet2irrlicht(const btVector3 &bulletVector);

template <typename Number>
int sign(Number num)
{
    if (num < 0)
        return -1;
    else if (num > 0)
        return 1;
    else
        return 0;
}

// GUI IDs
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
    ID_SPINBOX_RENDER_DISTANCE,

    GUI_IDS_COUNT};

constexpr f32 COLOR_CHANGE_INTERVAL = 3000.0f;

#endif // UTIL_H_INCLUDED
