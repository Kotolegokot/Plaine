#include "ObjMesh.h"

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

    enum { NONE, INT, FLOAT, KEYWORD, STRING, SPACE_BEFORE_STRING, COMMENT } state = NONE;
    while (inputFile) {
        int c = back ? back : inputFile.get();
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
                state = COMMENT;
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
                str.push_back(c);
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

        case COMMENT:
            if (c == '\n') {
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

void ObjMesh::loadMesh(const std::string &filename)
{
    std::vector<Item> items = parse(filename);
    for (const Item &item : items) {
        if (item.type == Item::SLASH)
            std::cout << "SLASH" << std::endl;
        else if (item.type == Item::INT)
            std::cout << "INT: " << (int) item.getInt() << std::endl;
        else if (item.type == Item::FLOAT)
            std::cout << "FLOAT: " << (float) item.getFloat() << std::endl;
        else if (item.type == Item::STRING)
            std::cout << "STRING: '" << item.getString() << "'" << std::endl;
        else if (item.type == Item::KEYWORD)
            std::cout << "KEYWORD: " << item.getString() << std::endl;
        else if (item.type == Item::NEWLINE)
            std::cout << "NEWLINE" << std::endl;
    }

    vertices.clear();
    triangles.clear();

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: unable to open file\"" << filename << "\" for reading" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);

        char c;
        iss >> c;

        if (line[0] == 'v' && line[1] == ' ') {
            btScalar x, y, z;
            iss >> x >> y >> z;
            btVector3 point(x, y, z);

            vertices.push_back(point);
        } else if (line[0] == 'f' && line[1] == ' ') {
            std::string s1, s2, s3;
            iss >> s1 >> s2 >> s3;

            s1 = s1.substr(0, s1.find('/'));
            s2 = s2.substr(0, s2.find('/'));
            s3 = s3.substr(0, s3.find('/'));

            btScalar t1, t2, t3;
            t1 = std::stof(s1) - 1;
            t2 = std::stof(s2) - 1;
            t3 = std::stof(s3) - 1;
            btVector3 triangle(t1, t2, t3);

            triangles.push_back(triangle);
        }
    }
}

btTriangleMesh *ObjMesh::getTriangleMesh()
{
    btTriangleMesh *triangleMesh = new btTriangleMesh();
    for (const btVector3 &triangle : triangles)
        triangleMesh->addTriangle(vertices[triangle.x()], vertices[triangle.y()], vertices[triangle.z()]);

    return triangleMesh;
}
