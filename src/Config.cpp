#include "Config.h"

using namespace irr;

struct Item {
    enum ItemType { INT, FLOAT, STRING, KEYWORD, OP_EQUAL, OP_COMMA, NEWLINE };

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
        case NEWLINE:
            return "new line or end of file";
        default:
            return "";
        }
    }

    ItemType type;
    void *data = nullptr;
};

std::vector<Item> parse(const std::string &filename)
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
            } else if (c == -1) {
                break;
            } else {
                std::cerr << "Error: config \"" << filename << "\" is invalid." << std::endl;
                return std::vector<Item>();
            }
            break;

        case INT:
            if (isdigit(c)) {
                str.push_back(c);
            } else if (c == '.') {
                str.push_back(c);
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

void error(Item::ItemType expected, Item::ItemType found)
{
    std::cout << "Error: " + Item::typeToString(expected) + " expected, but " +
        Item::typeToString(found) + " found." << std::endl;
}

#define \
EXPECT(_expected) {\
    if ((*i).type != _expected) {\
        error(_expected, (*i).type);\
        goToNextNEWLINE = true;\
        break;\
    }\
}

ConfigData Config::loadConfig(const std::string &filename)
{
    ConfigData data;

    std::vector<Item> items = parse(filename);

    // print items
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
    enum { NONE, RESOLUTION, FULLSCREEN, COLORDEPTH, LANGUAGE, RESIZABLE } state = NONE;

    for (std::vector<Item>::const_iterator i = items.cbegin(); i != items.cend(); ++i) {
        if (goToNextNEWLINE) {
            if ((*i).type == Item::NEWLINE)
                goToNextNEWLINE = false;
        } else {
            switch (state) {
            case NONE:
                if ((*i).type == Item::KEYWORD) {
                    if ((*i).getString() == "resolution")
                        state = RESOLUTION;
                    else if ((*i).getString() == "fullscreen")
                        state = FULLSCREEN;
                    else if ((*i).getString() == "colordepth")
                        state = COLORDEPTH;
                    else if ((*i).getString() == "language")
                        state = LANGUAGE;
                    else if ((*i).getString() == "resizable")
                        state = RESIZABLE;
                } else {
                    error(Item::KEYWORD, (*i).type);
                    goToNextNEWLINE = true;
                }
                break;
            case RESOLUTION: {
                core::dimension2d<u32> resolution;

                EXPECT(Item::OP_EQUAL);
                ++i;
                EXPECT(Item::INT);
                resolution.Width = (*i).getInt();
                ++i;
                EXPECT(Item::OP_COMMA);
                ++i;
                EXPECT(Item::INT);
                resolution.Height = (*i).getInt();
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
                if ((*i).getString() != "on" && (*i).getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString((*i).type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.fullscreen = (*i).getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case COLORDEPTH: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::INT);
                if ((*i).getInt() != 8 && (*i).getInt() != 16 && (*i).getInt() != 32) {
                    std::cerr << "Error: 16 or 32 expected, but " << Item::typeToString((*i).type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.colordepth = (*i).getInt();
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
                std::wstring wstr = utf8_to_wide((*i).getString());
                for (std::wstring::const_iterator si = wstr.cbegin(); si != wstr.cend(); ++si)
                    data.language.append(*si);

                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            case RESIZABLE: {
                EXPECT(Item::OP_EQUAL);
                ++i;

                EXPECT(Item::KEYWORD);
                if ((*i).getString() != "on" && (*i).getString() != "off") {
                    std::cerr << "Error: on or off expected, but " << Item::typeToString((*i).type) << " found." << std::endl;
                    goToNextNEWLINE = true;
                    break;
                }
                data.resizable = (*i).getString() == "on";
                ++i;
                EXPECT(Item::NEWLINE);

                state = NONE;
                break;
            }
            }
        }
    }

    return data;
}

void Config::saveConfig(const std::string &filename, const ConfigData &data)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for writing" << std::endl;
        return;
    }

    outputFile << "resolution=" << data.resolution.Width << "," << data.resolution.Height << std::endl;
    outputFile << "fullscreen=" << (data.fullscreen ? "on" : "off") << std::endl;
    outputFile << "colordepth=" << data.colordepth << std::endl;
    outputFile << "language=" << "\"" << wide_to_utf8(std::wstring(data.language.c_str())) << "\"" << std::endl;
    outputFile << "resizable=" << (data.resizable ? "on" : "off") << std::endl;
}
