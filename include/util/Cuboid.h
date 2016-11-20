#ifndef CUBOID_H
#define CUBOID_H

#include <algorithm>
#include "Vector3.h"

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
