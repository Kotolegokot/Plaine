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

#ifndef CUBOID_H
#define CUBOID_H

#include <algorithm>
#include "Vector3.hpp"

template <typename Num>
struct Cuboid {
    Cuboid() = default;

    Cuboid(const Vector3<Num> &p1, const Vector3<Num> &p2) :
        p1(p1), p2(p2) {}

    template <typename Num2>
    Cuboid(const Cuboid<Num2> &other) :
        p1(other.p1), p2(other.p2) {}

    template <typename Num2>
    Cuboid &operator =(const Cuboid<Num2> &other)
    {
        p1 = other.p1;
        p2 = other.p2;

        return *this;
    }

    template <typename Num2>
    bool operator ==(const Cuboid<Num2> &other) const
    {
        return p1 == other.p1 && p2 == other.p2;
    }

    template <typename Num2>
    bool operator !=(const Cuboid<Num2> &other) const
    {
        return !(*this == other);
    }

    template <typename Num2>
    auto operator /(Num2 n) const
    {
        return Cuboid(p1 / n, p2 / n);
    }

    template <typename Num2>
    Cuboid &operator /=(Num2 n)
    {
        p1 /= n;
        p2 /= n;

        return *this;
    }

    template <typename Num2>
    auto operator %(Num2 n) const
    {
        return Cuboid(p1 % n, p2 % n);
    }

    template <typename Num2>
    Cuboid &operator %=(Num2 n)
    {
        p1 %= n;
        p2 %= n;

        return *this;
    }

    template <typename Num2>
    auto operator *(Num2 n) const
    {
        return Cuboid(p1 * n, p2 * n);
    }

    template <typename Num2>
    Cuboid &operator *=(Num2 n)
    {
        p1 *= n;
        p2 *= n;

        return *this;
    }

    template <typename Num2>
    auto operator +(const Cuboid<Num2> &other) const
    {
        return Cuboid(p1 + other.p1, p2 + other.p2);
    }

    template <typename Num2>
    Cuboid &operator +=(const Cuboid<Num2> &other)
    {
        p1 += other.p1;
        p2 += other.p2;

        return *this;
    }

    template <typename Num2>
    auto operator -(const Cuboid<Num2> &other) const
    {
        return Cuboid(p1 - other.p1, p2 - other.p2);
    }

    template <typename Num2>
    Cuboid &operator -=(const Cuboid<Num2> &other)
    {
        p1 -= other.p1;
        p2 -= other.p2;

        return *this;
    }

    Vector3<Num> p1;
    Vector3<Num> p2;
};

#endif // CUBOID_H
