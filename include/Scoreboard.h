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

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <irrlicht.h>
#include "util.h"

using namespace irr;


class Scoreboard
{
public:
    static std::vector <s32> loadScore(const std::string &filename);
    static void saveScore(const std::string &filename, const std::vector <s32> &data);

private:
    struct Item {
        enum ItemType { INT, OP_SHARP, NEWLINE };

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
            case OP_SHARP:
                return "#";
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

#endif // SCOREBOARD_H
