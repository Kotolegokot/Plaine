/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <array>
#include "util/Vector3.hpp"

template <typename T, std::size_t N>
struct Array3 : public std::array<T, N * N * N> {
    Array3() = default;

    constexpr const T &at(const Vector3s &pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &at(const Vector3s &pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &operator [](const Vector3s &pos) const
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    T &operator [](const Vector3s &pos)
    {
        return std::array<T, N * N * N>::at(pos.x + pos.y * N + pos.z * N * N);
    }

    constexpr const T &at(std::size_t pos) const = delete;
    T &at(std::size_t pos) = delete;

    constexpr const T &operator [](std::size_t pos) const = delete;
    T &operator [](std::size_t pos) = delete;
};
