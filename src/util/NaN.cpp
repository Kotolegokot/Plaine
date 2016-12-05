#include "util/NaN.h"

void notNanAssert(const btQuaternion &quat)
{
    notNanAssert(quat.getX());
    notNanAssert(quat.getY());
    notNanAssert(quat.getZ());
    notNanAssert(quat.getW());
}

void notNanAssert(const btVector3 &vec)
{
    notNanAssert(vec.getX());
    notNanAssert(vec.getY());
    notNanAssert(vec.getZ());
}

void notNanAssert(const btTransform &transform)
{
    notNanAssert(transform.getOrigin());
    notNanAssert(transform.getRotation());
}
