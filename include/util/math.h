#ifndef MATH_H
#define MATH_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

using namespace irr;

template<typename T>
constexpr T PI = T(3.1415926535897932385);

// shit making it possible to compile with TDM-GCC under Windows
template<>
constexpr float PI<float> = 3.1415926535897932385;

core::vector3df quatToEulerRad(const btQuaternion &quat);
core::vector3df quatToEulerDeg(const btQuaternion &quat);

template <typename Number>
int sign(Number num)
{
    if (num < 0)
        return -1;
    else if (num > 0)
        return 1;
    else
        return 0;
}

#endif // MATH_H
