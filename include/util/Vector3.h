#ifndef VECTOR3_H
#define VECTOR3_H

#include <bullet/LinearMath/btVector3.h>
#include <irrlicht.h>

template <typename Num>
struct Vector3 {
    Vector3() = default;
    Vector3(Num x, Num y, Num z) :
        x(x), y(y), z(z) {}

    // copy constructors
    template <typename Num2>
    Vector3(const Vector3<Num2> &other) :
        x(other.x), y(other.y), z(other.z) {}

    Vector3(const btVector3 &other) :
        x(other.x()), y(other.y()), z(other.z()) {}

    template <typename Num2>
    Vector3(const irr::core::vector3d<Num2> &other) :
        x(other.X), y(other.Y), z(other.Z) {}

    // copy assignment operators
    template <typename Num2>
    Vector3 &operator =(const Vector3<Num2> &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }

    Vector3 &operator =(const btVector3 &other)
    {
        x = other.x();
        y = other.y();
        z = other.z();

        return *this;
    }

    template <typename Num2>
    Vector3 &operator =(const irr::core::vector3df &other)
    {
        x = other.X;
        y = other.Y;
        z = other.Z;

        return *this;
    }

    btVector3 toBulletVector3() const
    {
        return btVector3(x, y, z);
    }

    template <typename Num2>
    irr::core::vector3d<Num2> toIrrlichtVector() const
    {
        return irr::core::vector3d<Num2>(x, y, z);
    }

    template <typename Num2>
    bool operator ==(const Vector3<Num2> &other) const
    {
        return x == other.x &&
                y == other.y &&
                z == other.z;
    }

    template <typename Num2>
    bool operator !=(const Vector3<Num2> &other) const
    {
        return !(*this == other);
    }

    template <typename Num2>
    auto operator /(Num2 n) const
    {
        return Vector3(x / n, y / n, z / n);
    }

    template <typename Num2>
    Vector3 &operator /=(Num2 n)
    {
        x /= n;
        y /= n;
        z /= n;

        return *this;
    }

    template <typename Num2>
    auto operator %(Num2 n) const
    {
        return Vector3(x % n, y % n, z % n);
    }

    template <typename Num2>
    Vector3 &operator %=(Num2 n)
    {
        x %= n;
        y %= n;
        z %= n;

        return *this;
    }

    template <typename Num2>
    auto operator *(Num2 n) const
    {
        return Vector3(x * n, y * n, z * n);
    }

    template <typename Num2>
    Vector3 &operator *=(Num2 n)
    {
        x *= n;
        y *= n;
        z *= n;

        return *this;
    }

    template <typename Num2>
    auto operator +(const Vector3<Num2> &other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    template <typename Num2>
    Vector3 &operator +=(const Vector3<Num2> &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    template <typename Num2>
    auto operator -(const Vector3<Num2> &other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    template <typename Num2>
    Vector3 &operator -=(const Vector3<Num2> &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Num x = 0;
    Num y = 0;
    Num z = 0;
};

#endif // VECTOR3_H
