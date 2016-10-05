#ifndef OBJMESH_H
#define OBJMESH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class ObjMesh
{
public:
    ObjMesh() = default;
    void loadMesh(const std::string &filename);
    btTriangleMesh *getTriangleMesh();

private:
    std::vector<btVector3> vertices;
    std::vector<btVector3> triangles;

    struct Item {
        enum ItemType { SLASH, INT, FLOAT, KEYWORD, STRING, COMMENT, NEWLINE };

        Item(ItemType type) :
            type(type) {}

        Item(ItemType type, const std::string &text) :
            type(type), data((void *) new std::string(text)) {}

        Item(ItemType type, btScalar floatNumber) :
            type(type), data((void *) new btScalar(floatNumber)) {}

        Item(ItemType type, int intNumber) :
            type(type), data((void *) new int(intNumber)) {}

        std::string getString()
        {
            return *(std::string *) data;
        }

        const std::string &getString() const
        {
            return *(std::string *) data;
        }

        btScalar getFloat() const
        {
            return *(btScalar *) data;
        }

        int getInt() const
        {
            return *(int *) data;
        }

        static std::string typeToString(ItemType type)
        {
            switch (type) {
            case SLASH:
                return "slash";
            case INT:
                return "integer";
            case FLOAT:
                return "float";
            case STRING:
                return "string";
            case KEYWORD:
                return "keyword";
            case NEWLINE:
                return "new line or end of file";
            case COMMENT:
                return "comment";
            default:
                return "";
            }
        }

        void free()
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
            case COMMENT:
                delete (std::string *) data;
                break;
            default:
                break;
            }
        }

        ItemType type;
        void *data = nullptr;
    };
    static std::vector<Item> parse(const std::string &filename);
};

#endif // OBJMESH_H
