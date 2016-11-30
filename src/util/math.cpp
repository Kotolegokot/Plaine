#include "util/math.h"

core::vector3df quatToEulerRad(const btQuaternion &quat)
{
    core::vector3df result;
    core::quaternion(quat.x(), quat.y(), quat.z(), quat.w()).toEuler(result);

    return result;
}

core::vector3df quatToEulerDeg(const btQuaternion &quat)
{
    core::vector3df result;
    core::quaternion(quat.x(), quat.y(), quat.z(), quat.w()).toEuler(result);
    result *= core::RADTODEG;

    return result;
}
