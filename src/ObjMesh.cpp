#include "ObjMesh.h"

void ObjMesh::loadMesh(const std::string &filename)
{
    points.clear();

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
        if (!(line[0] == 'v' && line[1] == ' '))
            continue;

        btScalar x, y, z;
        iss >> x >> y >> z;
        btVector3 point(x, y, z);

        points.push_back(point);
    }
}

void ObjMesh::putToShape(btConvexHullShape *convexHullShape)
{
    for (const btVector3 &point : points)
        convexHullShape->addPoint(point);
}
