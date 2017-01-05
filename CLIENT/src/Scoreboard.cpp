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

#include "Scoreboard.hpp"

using namespace irr;

// parses the file into items like string, numbers etc.
// uses a simple deterministic finite automaton
std::vector<Scoreboard::Item> Scoreboard::parse(const std::string &filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for reading" << std::endl;
        return std::vector<Item>();
    }

    std::vector<Item> items;
    std::string str;
    int back = 0;

    enum { NONE, INT, } state = NONE;
    while (inputFile) {
        int c = back ? back : inputFile.get();
        back = 0;
        switch (state) {
        case NONE:
            if (c == '\n') {
                items.push_back(Item(Item::NEWLINE));
            } else if (c == '#') {
                items.push_back(Item(Item::OP_SHARP));
            } else if (isspace(c)) {
                // nothing
            } else if (isdigit(c)) {
                str.push_back(c);
                state = INT;
            } else if (c == EOF) {
                break;
            } else {
                std::cerr << "Error: config \"" << filename << "\" is invalid." << std::endl;
                return std::vector<Item>();
            }
            break;

        case INT:
            if (isdigit(c)) {
                str.push_back(c);
            } else {
                back = c;
                items.push_back(Item(Item::INT, (s32) std::stoi(str)));
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

void Scoreboard::error(Item::ItemType expected, Item::ItemType found)
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

// loads score infotmation from file
// uses another simple deterministic finit automaton
std::vector<s32> Scoreboard::loadScore(const std::string &filename)
{
    std::vector<s32> data;

    std::vector<Item> items = parse(filename);

     //print items (for debug purposes)
     /*for (const Item &item : items) {
        if (item.type == Item::INT)
            std::cout << "INT: " << (int) item.getInt() << std::endl;
        else if (item.type == Item::OP_SHARP)
            std::cout << "OP_SHARP" << std::endl;
        else if (item.type == Item::NEWLINE)
            std::cout << "NEWLINE" << std::endl;
    }*/

    bool goToNextNEWLINE = false;
    enum { NONE, SCORE } state = NONE;

    for (std::vector<Item>::const_iterator i = items.cbegin(); i != items.cend(); ++i) {
        if (goToNextNEWLINE) {
            if (i->type == Item::NEWLINE)
                goToNextNEWLINE = false;
        } else {
            switch (state) {
            case NONE:
                EXPECT(Item::OP_SHARP);
                if (i->type == Item::OP_SHARP)
                    state = SCORE;
                break;
            case SCORE: {

                EXPECT(Item::INT);
                ++i;
                EXPECT(Item::INT);
                data.push_back(i->getInt());
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

// saves scores to file
// the algorithm is pretty much straightforward
void Scoreboard::saveScore(const std::string &filename, const std::vector<s32> &data)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for writing" << std::endl;
        return;
    }

    for (std::size_t i = 0; i < data.size(); i++)
        outputFile << "#" << std::to_string(i + 1) << " " << std::to_string(data[i]) << std::endl;
}
