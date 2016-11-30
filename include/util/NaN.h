#ifndef NAN_H
#define NAN_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

using namespace irr;

#if NAN_ASSERT
template <typename Real>
void notNanAssert(Real real)
{
    assert(!std::isnan(real));
}
#else
template <typename Real>
void notNanAssert(Real) {}
#endif

template <typename ...Ts>
void notNanAssert(const Ts &... params)
{
    // this line just call notNanAssert for all params
    (void) (int []) { 0, (notNanAssert(params), 0)... };
}

void notNanAssert(const btQuaternion &quat);
void notNanAssert(const btVector3 &vec);
void notNanAssert(const btTransform &transform);

#endif // NAN_H
