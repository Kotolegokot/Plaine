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

#ifndef _WIN32
#include <iconv.h>
#else
#include <Windows.h>
#endif

using namespace irr;

#define _(string) gettext(string)
#define _w(string) utf8_to_irrwide(gettext(string))
#define _wp(string) utf8_to_wide(gettext(string)).c_str()

template<typename T>
constexpr T PI = T(3.1415926535897932385);

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
core::stringw utf8_to_irrwide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);
core::stringw keyCodeName(const EKEY_CODE &keyCode);
void setLanguage(std::string language, bool replace);
video::SColor iridescentColor(const u32 &currentTime);

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

template <typename Num>
struct Point3 {
    Point3() = default;
    Point3(Num x, Num y, Num z) :
        x(x), y(y), z(z) {}

    template <typename Num2>
    Point3(const Point3<Num2> &other) :
        x(other.x), y(other.y), z(other.z) {}

    template <typename Num2>
    bool operator ==(const Point3<Num2> &other) const
    {
        return x == other.x &&
                y == other.y &&
                z == other.z;
    }

//    Point3<Num> operator

    template <typename Num2>
    Point3<Num> &operator /=(Num2 n)
    {
        x /= n;
        y /= n;
        z /= n;

        return *this;
    }

    template <typename Num2>
    Point3<Num> &operator %=(Num2 n)
    {
        x %= n;
        y %= n;
        z %= n;

        return *this;
    }

    template <typename Num2, typename Num3>
    friend Point3 operator +(const Point3<Num2> &p1, const Point3<Num3> &p2)
    {
        return Point3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }

    template <typename Num2, typename Num3>
    friend Point3 operator -(const Point3<Num2> &p1, const Point3<Num3> &p2)
    {
        return Point3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    template <typename Num2, typename Num3>
    friend Point3 operator *(const Point3<Num2> &p1, Num3 n)
    {
        return Point3(p1.x * n, p1.y * n, p1.z * n);
    }

    template <typename Num2, typename Num3>
    friend Point3 operator /(const Point3<Num2> &p1, Num3 n)
    {
        return Point3(p1.x / n, p1.y / n, p1.z / n);
    }

    template <typename Num2, typename Num3>
    friend Point3 operator %(const Point3<Num2> &p1, Num3 n)
    {
        return Point3(p1.x % n, p1.y % n, p1.z % n);
    }

    Num x = 0;
    Num y = 0;
    Num z = 0;
};

//template <typename Num>
//struct Cube {
//    Cube() = default;

//    Cube(const Point3<Num> &p1, const Point3<Num> &p2) :
//        p1(p1), p2(p2) {}

//    template <typename Num2>
//    bool has(const Point3<Num2> &p)
//    {
//        return (p.x <= std::max(p1.x, p2.x) && p.x >= std::min(p1.x, p2.x)) &&
//               (p.y <= std::max(p1.y, p2.y) && p.y >= std::min(p1.y, p2.y)) &&
//               (p.z <= std::max(p1.z, p2.z) && p.z >= std::min(p1.z, p2.z));
//    }

//    Point3<Num> p1;
//    Point3<Num> p2;
//};

template <typename T, std::size_t N>
struct Array3 : public std::array<T, N * N * N> {
    Array3() = default;

    constexpr const T &at(Point3<std::size_t> pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &at(Point3<std::size_t> pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &at(std::size_t pos) const = delete;
    T &at(std::size_t pos) = delete;

    constexpr const T &operator [](std::size_t pos) const = delete;
    T &operator [](std::size_t pos) = delete;
};

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
