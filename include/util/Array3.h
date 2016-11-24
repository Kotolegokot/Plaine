/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARRAY3_H
#define ARRAY3_H

#include <array>

template <typename T, std::size_t N>
struct Array3 : public std::array<T, N * N * N> {
    Array3() = default;

    constexpr const T &at(const Vector3<std::size_t> &pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &at(const Vector3<std::size_t> &pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &operator [](const Vector3<std::size_t> &pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &operator [](const Vector3<std::size_t> &pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &at(std::size_t pos) const = delete;
    T &at(std::size_t pos) = delete;

    constexpr const T &operator [](std::size_t pos) const = delete;
    T &operator [](std::size_t pos) = delete;
};

#endif // ARRAY3_H
