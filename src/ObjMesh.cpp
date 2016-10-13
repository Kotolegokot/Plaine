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

#include "ObjMesh.h"

ObjMesh::ObjMesh(const std::string &filename)
{
    loadMesh(filename);
}

std::vector<ObjMesh::Item> ObjMesh::parse(const std::string &filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for reading" << std::endl;
        return std::vector<Item>();
    }

    std::vector<Item> items;
    std::string str;
    int back = 0;

    enum { NONE, INT, FLOAT, KEYWORD, STRING, SPACE_BEFORE_STRING, COMMENT, SPACE_BEFORE_COMMENT } state = NONE;
    while (inputFile) {
        int c = (back ? back : inputFile.get());
        back = 0;
        switch (state) {
        case NONE:
            if (c == '\n') {
                items.push_back(Item(Item::NEWLINE));
            } else if (isspace(c)) {
                // nothing
            } else if (c == '/') {
                items.push_back(Item(Item::SLASH));
            } else if (c == '-' || isdigit(c)) {
                str.push_back(c);
                state = INT;
            } else if (isalpha(c)) {
                str.push_back(c);
                state = KEYWORD;
            } else if (c == '#') {
                state = SPACE_BEFORE_COMMENT;
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
            } else if (c == '.') {
                str.append(localeconv()->decimal_point);
                state = FLOAT;
            } else {
                back = c;
                items.push_back(Item(Item::INT, std::stoi(str)));
                str.clear();
                state = NONE;
            }
            break;

        case FLOAT:
            if (isdigit(c)) {
                str.push_back(c);
            } else {
                back = c;
                items.push_back(Item(Item::FLOAT, std::stof(str)));
                str.clear();
                state = NONE;
            }
            break;

        case KEYWORD:
            if (isalnum(c)) {
                str.push_back(c);
            } else {
                items.push_back(Item(Item::KEYWORD, str));
                state = (str == "mtllib" || str == "usemtl" || str == "o" || str == "g")
                    ? SPACE_BEFORE_STRING : NONE;
                back = c;
                str.clear();
            }
            break;

        case SPACE_BEFORE_STRING:
            if (!isspace(c)) {
                back = c;
                state = STRING;
            }
            break;

        case STRING:
            if (c == '\n' || c == EOF) {
                back = c;
                items.push_back(Item(Item::STRING, str));
                str.clear();
                state = NONE;
            } else {
                str.push_back(c);
            }
            break;

        case SPACE_BEFORE_COMMENT:
            if (!isspace(c)) {
                back = c;
                state = COMMENT;
            }
            break;

        case COMMENT:
            if (c == '\n' || c == EOF) {
                back = c;
                items.push_back(Item(Item::COMMENT, str));
                str.clear();
                state = NONE;
            } else {
                str.push_back(c);
            }
        }
    }

    if (items.back().type != Item::NEWLINE)
        items.push_back(Item(Item::NEWLINE));

    return items;
}

void ObjMesh::error(Item::ItemType expected, Item::ItemType found)
{
    std::cout << "Error: " + Item::typeToString(expected) + " expected, but " +
        Item::typeToString(found) + " found." << std::endl;
}

#define \
EXPECT(_expected) {\
    if ((*i).type != _expected) {\
        error(_expected, (*i).type);\
        goToNextNEWLINE = true;\
        state = NONE;\
        break;\
    }\
}

void ObjMesh::loadMesh(const std::string &filename)
{
    vertices.clear();
    polygons.clear();

    std::vector<Item> items = parse(filename);

    /*for (const Item &item : items) {
        if (item.type == Item::SLASH)
            std::cout << "SLASH" << std::endl;
        else if (item.type == Item::INT)
            std::cout << "INT: " << (int) item.getInt() << std::endl;
        else if (item.type == Item::FLOAT)
            std::cout << "FLOAT: " << (float) item.getFloat() << std::endl;
        else if (item.type == Item::STRING)
            std::cout << "STRING: '" << item.getString() << "'" << std::endl;
        else if (item.type == Item::COMMENT)
            std::cout << "COMMENT: '" << item.getString() << "'" << std::endl;
        else if (item.type == Item::KEYWORD)
            std::cout << "KEYWORD: " << item.getString() << std::endl;
        else if (item.type == Item::NEWLINE)
            std::cout << "NEWLINE" << std::endl;
    }*/

    bool goToNextNEWLINE = false;
    enum { NONE, VERTEX, FACE } state = NONE;

    for (std::vector<Item>::const_iterator i = items.cbegin(); i != items.cend(); ++i) {
        if (goToNextNEWLINE) {
            if (i->type == Item::NEWLINE)
                goToNextNEWLINE = false;
        } else {
            switch (state) {
            case NONE:
                if (i->type == Item::COMMENT) {
                    goToNextNEWLINE = true;
                    break;
                }
                EXPECT(Item::KEYWORD);

                if (i->getString() == "v") {
                    state = VERTEX;
                } else if (i->getString() == "f") {
                    state = FACE;
                } else {
                    goToNextNEWLINE = true;
                }
                break;

            case VERTEX:
                {
                    btVector3 vertex;

                    EXPECT(Item::FLOAT);
                    vertex.setX(-i->getFloat());
                    ++i;

                    EXPECT(Item::FLOAT);
                    vertex.setY(i->getFloat());
                    ++i;

                    EXPECT(Item::FLOAT);
                    vertex.setZ(i->getFloat());
                    ++i;

                    if (i->type == Item::FLOAT)
                        ++i;
                    EXPECT(Item::NEWLINE);

                    vertices.push_back(vertex);

                    state = NONE;
                }
                break;

            case FACE:
                {
                    std::vector<size_t> polygon;

                    while (i->type != Item::NEWLINE) {
                        EXPECT(Item::INT);
                        polygon.push_back(i->getInt() - 1);
                        ++i;

                        if (i->type == Item::SLASH) {
                            ++i;
                            if (i->type == Item::SLASH) {
                                ++i;
                                EXPECT(Item::INT);
                                ++i;
                            } else if (i->type == Item::INT) {
                                ++i;
                                if (i->type == Item::SLASH) {
                                    ++i;
                                    EXPECT(Item::INT);
                                    ++i;
                                }
                            } else {
                                std::cout << "Error: " + Item::typeToString(Item::SLASH) + " or " +
                                    Item::typeToString(Item::INT) + " expected, but " +
                                    Item::typeToString(i->type) + " found." << std::endl;
                                state = NONE;
                                goToNextNEWLINE = true;
                                break;
                            }
                        }
                    }

                    polygons.push_back(polygon);

                    state = NONE;
                }
                break;
            }
        }
    }

    /*std::ofstream out(filename + "_");;
    for (std::vector<btVector3>::const_iterator i = vertices.cbegin(); i != vertices.cend(); i++)
        out << "v " << i->x() << " " << i->y() << " " << i->z() << std::endl;

    for (std::vector<std::vector<size_t>>::const_iterator i = polygons.cbegin(); i != polygons.cend(); i++) {
        out << "f ";
        for (std::vector<size_t>::const_iterator j = i->cbegin(); j != i->cend(); j++)
            out << *j << " ";
        out << std::endl;
    }*/
}

std::unique_ptr<btTriangleMesh> ObjMesh::getTriangleMesh()
{
    auto triangleMesh = std::make_unique<btTriangleMesh>();
    for (const std::vector<size_t> &polygon : polygons)
        for (size_t i = 1; i < polygon.size() - 1; i++)
            triangleMesh->addTriangle(vertices[polygon[0]], vertices[polygon[i]], vertices[polygon[i + 1]]);

    return triangleMesh;
}

void ObjMesh::setPoints(btConvexHullShape &shape)
{
    for (const btVector3 &vertex : vertices)
        shape.addPoint(vertex);
}
