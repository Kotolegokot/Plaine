#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <btBulletDynamicsCommon.h>

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;

constexpr btScalar MASS_COEFFICIENT = 0.000002;

template<typename T>
constexpr T PI = T(3.1415926535897932385);

// shit making it possible to compile with TDM-GCC under Windows
template<>
constexpr float PI<float> = 3.1415926535897932385;

#endif // CONSTANTS_H
