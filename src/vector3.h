#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3
{
public:
    typedef float IMPL_TYPE;

    Vector3() : m_x(0.0), m_y(0.0), m_z(0.0) {}
    Vector3(const Vector3& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}
    Vector3(const Vector3&& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}
    Vector3(IMPL_TYPE x, IMPL_TYPE y, IMPL_TYPE z) : m_x(x), m_y(y), m_z(z) {}

    inline IMPL_TYPE GetX() const { return m_x; }
    inline IMPL_TYPE GetY() const { return m_y; }
    inline IMPL_TYPE GetZ() const { return m_z; }

    inline void SetX(IMPL_TYPE x) { m_x = x; }
    inline void SetY(IMPL_TYPE y) { m_y = y; }
    inline void SetZ(IMPL_TYPE z) { m_z = z; }

    IMPL_TYPE Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;
    bool HasNaNs() const;

    IMPL_TYPE GetLength() const;
    IMPL_TYPE GetLengthSquared() const;
    Vector3 GetNormalized() const;
    Vector3 GetNegative() const;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator/(const Vector3& other) const;

    Vector3& operator=(const Vector3& other);
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(const Vector3& other);
    Vector3& operator/=(const Vector3& other);

    Vector3 operator*(const IMPL_TYPE other) const;
    Vector3 operator/(const IMPL_TYPE other) const;
    Vector3& operator*=(const IMPL_TYPE other);
    Vector3& operator/=(const IMPL_TYPE other);

    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

private:
    IMPL_TYPE m_x;
    IMPL_TYPE m_y;
    IMPL_TYPE m_z;
};

#endif   // VECTOR3_H
