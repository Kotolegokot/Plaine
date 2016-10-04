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
};

#endif // OBJMESH_H
