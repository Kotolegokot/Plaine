#include "ObjMesh.h"

void ObjMesh::loadMesh(const std::string &filename)
{
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
