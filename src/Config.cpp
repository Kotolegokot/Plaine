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

#include "Config.h"

using namespace irr;

// parses the file into items like string, numbers etc.
// uses a simple deterministic finite automaton
std::vector<Config::Item> Config::parse(const std::string &filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for reading" << std::endl;
        return std::vector<Item>();
    }

    std::vector<Item> items;
    std::string str;
    int back = 0;

    enum { NONE, INT, FLOAT, STRING, KEYWORD } state = NONE;
    while (inputFile) {
        int c = back ? back : inputFile.get();
        back = 0;
        switch (state) {
        case NONE:
            if (c == '\n') {
                items.push_back(Item(Item::NEWLINE));
            } else if (c == '=') {
                items.push_back(Item(Item::OP_EQUAL));
            } else if (c == ',') {
                items.push_back(Item(Item::OP_COMMA));
            } else if (c == ':') {
                items.push_back(Item(Item::OP_COLON));
            } else if (isspace(c)) {
                // nothing
            } else if (c == '"') {
                state = STRING;
            } else if (isalpha(c)) {
                str.push_back(c);
                state = KEYWORD;
            } else if (isdigit(c)) {
                str.push_back(c);
                state = INT;
            } else if (c == EOF) {
                break;
            } else {
                std::cerr << "Error: config \"" << filename << "\" is invalid." << std::endl;
                for (Item &item : items)
                    item.free();
                return std::vector<Item>();
            }
            break;

        case INT:
            if (isdigit(c)) {
                str.push_back(c);
            } else if (c == '.') {
                str.append(localeconv()->decimal_point);
                state = FLOAT;
            } else {
                back = c;
                items.push_back(Item(Item::INT, (s32) std::stoi(str)));
                str.clear();
                state = NONE;
            }
            break;

        case FLOAT:
            if (isdigit(c)) {
                str.push_back(c);
            } else {
                back = c;
                items.push_back(Item(Item::FLOAT, (f32) stof(str)));
                str.clear();
                state = NONE;
            }

        case STRING:
            if (c == '"') {
                items.push_back(Item(Item::STRING, str));
                str.clear();
                state = NONE;
            } else {
                str.push_back(c);
            }
            break;

        case KEYWORD:
            if (isalnum(c)) {
                str.push_back(c);
            } else {
                back = c;
                items.push_back(Item(Item::KEYWORD, str));
                str.clear();
                state = NONE;
            }
            break;
        }
    }

    if (items.back().type != Item::NEWLINE)
        items.push_back(Item(Item::NEWLINE));

    return items;
}

void Config::error(Item::ItemType expected, Item::ItemType found)
{
    std::cout << "Error: " + Item::typeToString(expected) + " expected, but " +
        Item::typeToString(found) + " found." << std::endl;
}

// throws an error if we encounter something we don't want
#define \
EXPECT(_expected) {\
    if (i->type != _expected) {\
        error(_expected, i->type);\
        goToNextNEWLINE = true;\
        state = NONE;\
        break;\
    }\
}

// loads configuration infotmation from file
// uses another simple deterministic finit automaton
ConfigData Config::loadConfig(const std::string &filename)
{
    ConfigData data;

    std::vector<Item> items = parse(filename);

    // print items (for debug purposes)
    /* for (const Item &item : items) {
        if (item.type == Item::INT)
            std::cout << "INT: " << (int) item.getInt() << std::endl;
        else if (item.type == Item::FLOAT)
            std::cout << "FLOAT: " << (float) item.getFloat() << std::endl;
        else if (item.type == Item::STRING)
            std::cout << "STRING: '" << item.getString() << "'" << std::endl;
        else if (item.type == Item::KEYWORD)
            std::cout << "KEYWORD: " << item.getString() << std::endl;
        else if (item.type == Item::OP_EQUAL)
            std::cout << "OP_EQUAL" << std::endl;
        else if (item.type == Item::OP_COMMA)
            std::cout << "OP_COMMA" << std::endl;
        else if (item.type == Item::NEWLINE)
            std::cout << "NEWLINE" << std::endl;
    } */

    bool goToNextNEWLINE = false;
    enum { NONE, RESOLUTION, FULLSCREEN, LANGUAGE, RESIZABLE, VSYNC, STENCILBUFFER, RENDER_DISTANCE, CONTROLS,
    CONTROL_UP, CONTROL_LEFT, CONTROL_DOWN, CONTROL_RIGHT, CONTROL_CW_ROLL, CONTROL_CCW_ROLL} state = NONE;

    for (std::vector<Item>::const_iterator i = items.cbegin(); i != items.cend(); ++i) {
        if (goToNextNEWLINE) {
            if (i->type == Item::NEWLINE)
                goToNextNEWLINE = false;
        } else {
            switch (state) {
            case NONE:
                EXPECT(Item::KEYWORD);

                if (i->getString() == "resolution")
                    state = RESOLUTION;
                else if (i->getString() == "fullscreen")
                    state = FULLSCREEN;
                else if (i->getString() == "language")
                    state = LANGUAGE;
                else if (i->getString() == "resizable")
                    state = RESIZABLE;
                else if (i->getString() == "vsync")
                    state = VSYNC;
                else if (i->getString() == "stencilbuffer")
                    state = STENCILBUFFER;
                else if (i->getString() == "renderdistance")
                    state = RENDER_DISTANCE;
                else if (i->getString() == "controls")
                    state = CONTROLS;
                else if (i->getString() == "up")
                    state = CONTROL_UP;
                else if (i->getString() == "left")
                    state = CONTROL_LEFT;
                else if (i->getString() == "down")
                    state = CONTROL_DOWN;
                else if (i->getString() == "right")
                    state = CONTROL_RIGHT;
                else if (i->getString() == "clockwiseroll")
                    state = CONTROL_CW_ROLL;
                else if (i->getString() == "counterclockwiseroll")
                    state = CONTROL_CCW_ROLL;
                break;
            case RESOLUTION: {
                core::dimension2d<u32> resolution;

                EXPECT(Item::OP_EQUAL);
                ++i;
                EXPECT(Item::INT);
                resolution.Width = i->getInt();
                ++i;
                EXPECT(Item::OP_COMMA);
                ++i;
                EXPECT(Item::INT);
                resolution.Height = i->getInt();
                data.resolution = resolution;
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case FULLSCREEN: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::KEYWORD);
                if (i->getString() != "on" && i->getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString(i->type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.fullscreen = i->getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case LANGUAGE: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::STRING);
                data.language = "";
                std::string str = i->getString();
                for (std::string::const_iterator si = str.cbegin(); si != str.cend(); ++si)
                    data.language.push_back(*si);

                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case RESIZABLE: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::KEYWORD);
                if (i->getString() != "on" && i->getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString(i->type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.resizable = i->getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case VSYNC: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::KEYWORD);
                if (i->getString() != "on" && i->getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString(i->type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.vsync = i->getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case STENCILBUFFER: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::KEYWORD);
                if (i->getString() != "on" && i->getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString(i->type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.stencilBuffer = i->getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case RENDER_DISTANCE: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.renderDistance = i->getInt();
                ++i;

                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROLS: {
                EXPECT(Item::OP_COLON);
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_UP: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.up = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_LEFT: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.left = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_DOWN: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.down = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_RIGHT: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.right = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_CW_ROLL: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.cwRoll = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case CONTROL_CCW_ROLL: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                data.controls.ccwRoll = (EKEY_CODE)i->getInt();
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            }
        }
    }

    for (Item &item : items)
        item.free();

    return data;
}

// saves configuration to file
// the algorithm is pretty much straightforward
void Config::saveConfig(const std::string &filename, const ConfigData &data)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for writing" << std::endl;
        return;
    }

    outputFile << "resolution=" << data.resolution.Width << "," << data.resolution.Height << std::endl;
    outputFile << "fullscreen=" << (data.fullscreen ? "on" : "off") << std::endl;
    outputFile << "language=" << "\"" << std::string(data.language.c_str()) << "\"" << std::endl;
    outputFile << "resizable=" << (data.resizable ? "on" : "off") << std::endl;
    outputFile << "vsync=" << (data.vsync ? "on" : "off") << std::endl;
    outputFile << "stencilbuffer=" << (data.stencilBuffer ? "on" : "off") << std::endl;
    outputFile << "renderdistance=" << data.renderDistance << std::endl;
    outputFile << "controls:" << std::endl;
    outputFile << "    up=" << data.controls.up << std::endl;
    outputFile << "    left=" << data.controls.left << std::endl;
    outputFile << "    down=" << data.controls.down << std::endl;
    outputFile << "    right=" << data.controls.right << std::endl;
    outputFile << "    clockwiseroll=" << data.controls.cwRoll << std::endl;
    outputFile << "    counterclockwiseroll=" << data.controls.ccwRoll << std::endl;
}
