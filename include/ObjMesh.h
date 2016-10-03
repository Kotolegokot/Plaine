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
    void putToShape(btConvexHullShape *convexHullShape);

private:
    std::vector<btVector3> points;
};

#endif // OBJMESH_H
