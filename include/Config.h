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

#ifndef CONFIG_H
#define CONFIG_H

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <irrlicht.h>
#include "util.h"

using namespace irr;

constexpr std::size_t CONTROLS_COUNT = 6;
struct CONTROL {
    enum { UP, LEFT, DOWN, RIGHT, CW_ROLL, CCW_ROLL };
};

class Controls {
public:
    Controls()
    {
        keyCodes[CONTROL::UP] = KEY_KEY_W;
        keyCodes[CONTROL::LEFT] = KEY_KEY_A;
        keyCodes[CONTROL::DOWN] = KEY_KEY_S;
        keyCodes[CONTROL::RIGHT] = KEY_KEY_D;
        keyCodes[CONTROL::CW_ROLL] = KEY_KEY_E;
        keyCodes[CONTROL::CCW_ROLL] = KEY_KEY_Q;
    }

    EKEY_CODE &operator[](std::size_t i)
    {
        return keyCodes[i];
    }

    EKEY_CODE operator[](std::size_t i) const
    {
        return keyCodes[i];
    }
private:
    std::array<EKEY_CODE, CONTROLS_COUNT> keyCodes;
};

// structure containing all the configuration info
struct ConfigData {
    bool fullscreen = false;
    core::dimension2d<u32> resolution = core::dimension2d<u32>(640, 480);
    std::string language = "c";
    bool resizable = false;
    bool vsync = true;
    bool stencilBuffer = true;
    u32 renderDistance = 2000;
    Controls controls;

    bool needRestart(const ConfigData &another) const
    {
        return another.vsync != vsync ||
            another.stencilBuffer != stencilBuffer ||
            another.fullscreen != fullscreen ||
            another.resizable != resizable ||
            another.resolution != resolution;
    }
};

// this class helps save and load configuration files
// usually it is ./game.conf
//
// its format is easy:
//      var=expression
//      ...
// expression can be either a string ("bla-bla") or
//      a boolean (on/off) or an integer or
//      a float or a resolution (int,int)
// also controls are set like this:
//      controls:
//          key1=keyCode
//          key2=keyCode
//          ...
class Config
{
public:
    static ConfigData loadConfig(const std::string &filename);
    static void saveConfig(const std::string &filename, const ConfigData &data);

private:
    struct Item {
        enum ItemType { INT, FLOAT, STRING, KEYWORD, OP_EQUAL, OP_COMMA, OP_COLON, NEWLINE };

        Item(ItemType type) :
            type(type) {}

        Item(ItemType type, const std::string &text) :
            type(type), data((void *) new std::string(text)) {}

        Item(ItemType type, f32 floatNumber) :
            type(type), data((void *) new f32)
        {
            *(f32 *) data = floatNumber;
        }

        Item(ItemType type, s32 intNumber) :
            type(type), data((void *) new s32)
        {
            *(s32 *) data = intNumber;
        }

        Item(Item &&item) noexcept :
            type(item.type), data(item.data)
        {
            item.data = nullptr;
        }

        ~Item()
        {
            switch (type) {
            case INT:
                delete (int *) data;
                break;
            case FLOAT:
                delete (float *) data;
                break;
            case STRING:
            case KEYWORD:
                delete (std::string *) data;
                break;
            default:
                break;
            }
        }

        std::string getString()
        {
            return *(std::string *) data;
        }

        const std::string &getString() const
        {
            return *(std::string *) data;
        }

        f32 getFloat() const
        {
            return *(f32 *) data;
        }

        s32 getInt() const
        {
            return *(s32 *) data;
        }

        static std::string typeToString(ItemType type)
        {
            switch (type) {
            case INT:
                return "integer";
            case FLOAT:
                return "float";
            case STRING:
                return "string";
            case KEYWORD:
                return "keyword";
            case OP_EQUAL:
                return "'='";
            case OP_COMMA:
                return "comma";
            case OP_COLON:
                return ":";
            case NEWLINE:
                return "new line or end of file";
            default:
                return "";
            }
        }

        ItemType type;
        void *data = nullptr;
    };

    static std::vector<Item> parse(const std::string &filename);
    static void error(Item::ItemType expected, Item::ItemType found);
};

#endif // CONFIG_H
