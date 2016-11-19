#ifndef VECTOR3_H
#define VECTOR3_H

template <typename Num>
struct Vector3 {
    Vector3() = default;
    Vector3(Num x, Num y, Num z) :
        x(x), y(y), z(z) {}

    // copy constructors
    template <typename Num2>
    Vector3(const Vector3<Num2> &other) :
        x(other.x), y(other.y), z(other.z) {}

    // move constructors
    template <typename Num2>
    Vector3(Vector3<Num2> &&other) :
        x(other.x), y(other.y), z(other.z) {}

    // move assignment operator
    template <typename Num2>
    Vector3 &operator =(Vector3<Num2> &&other)
    {
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }

    template <typename Num2>
    bool operator ==(const Vector3<Num2> &other) const
    {
        return x == other.x &&
                y == other.y &&
                z == other.z;
    }

    template <typename Num2, typename Num3>
    friend Vector3 operator /(const Vector3<Num2> &p1, Num3 n)
    {
        return Vector3(p1.x / n, p1.y / n, p1.z / n);
    }

    template <typename Num2>
    Vector3 &operator /=(Num2 n)
    {
        x /= n;
        y /= n;
        z /= n;

        return *this;
    }

    template <typename Num2, typename Num3>
    friend Vector3 operator %(const Vector3<Num2> &p1, Num3 n)
    {
        return Vector3(p1.x % n, p1.y % n, p1.z % n);
    }

    template <typename Num2>
    Vector3 &operator %=(Num2 n)
    {
        x %= n;
        y %= n;
        z %= n;

        return *this;
    }

    template <typename Num2, typename Num3>
    friend Vector3 operator *(const Vector3<Num2> &p1, Num3 n)
    {
        return Vector3(p1.x * n, p1.y * n, p1.z * n);
    }

    template <typename Num2>
    Vector3 &operator *=(Num2 n)
    {
        x *= n;
        y *= n;
        z *= n;

        return *this;
    }

    template <typename Num2, typename Num3>
    friend Vector3 operator +(const Vector3<Num2> &p1, const Vector3<Num3> &p2)
    {
        return Vector3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }

    template <typename Num2>
    Vector3 &operator +=(Num2 n)
    {
        x += n;
        y += n;
        z += n;

        return *this;
    }

    template <typename Num2, typename Num3>
    friend Vector3 operator -(const Vector3<Num2> &p1, const Vector3<Num3> &p2)
    {
        return Vector3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    template <typename Num2>
    Vector3 &operator -=(Num2 n)
    {
        x -= n;
        y -= n;
        z -= n;
    }

    Num x = 0;
    Num y = 0;
    Num z = 0;
};

#endif // VECTOR3_H
