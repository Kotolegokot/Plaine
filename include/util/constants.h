#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <btBulletDynamicsCommon.h>

constexpr std::size_t CHUNK_SIZE = 16;
constexpr std::size_t CHUNK_DB_SIZE = 200;
constexpr btScalar CELL_LENGTH = 250;
constexpr btScalar CHUNK_LENGTH = CHUNK_SIZE * CELL_LENGTH;

constexpr btScalar MASS_COEFFICIENT = 0.000002;


#endif // CONSTANTS_H
