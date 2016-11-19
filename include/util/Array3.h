#ifndef ARRAY3_H
#define ARARY3_H

#include <array>

template <typename T, std::size_t N>
struct Array3 : public std::array<T, N * N * N> {
    Array3() = default;

    constexpr const T &at(Vector3<std::size_t> pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &at(Vector3<std::size_t> pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &at(std::size_t pos) const = delete;
    T &at(std::size_t pos) = delete;

    constexpr const T &operator [](std::size_t pos) const = delete;
    T &operator [](std::size_t pos) = delete;
};

#endif ARRAY3_H
