#include "vector3.h"

Vector3::IMPL_TYPE Vector3::Dot(const Vector3& other) const
{
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
    IMPL_TYPE a = m_y * other.m_z - m_z * other.m_y;
    IMPL_TYPE b = m_z * other.m_x - m_x * other.m_z;
    IMPL_TYPE c = m_x * other.m_y - m_y * other.m_x;
    return Vector3(a, b, c);
}

Vector3::IMPL_TYPE Vector3::GetLength() const
{
    return std::sqrt(GetLengthSquared());
}

Vector3::IMPL_TYPE Vector3::GetLengthSquared() const
{
    return ((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

Vector3 Vector3::GetNormalized() const
{
    IMPL_TYPE mag = GetLength();
    return Vector3(m_x / mag, m_y / mag, m_z / mag);
}

Vector3 Vector3::GetNegative() const
{
    return Vector3(-m_x, -m_y, -m_z);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector3 Vector3::operator*(const Vector3& other) const
{
    return Vector3(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
}

Vector3 Vector3::operator/(const Vector3& other) const
{
    return Vector3(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
}

Vector3& Vector3::operator=(const Vector3& other)
{
    m_x = other.m_x;
    m_y = other.m_y;
    m_z = other.m_z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
}
Vector3& Vector3::operator-=(const Vector3& other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    return *this;
}
Vector3& Vector3::operator*=(const Vector3& other)
{
    m_x *= other.m_x;
    m_y *= other.m_y;
    m_z *= other.m_z;
    return *this;
}
Vector3& Vector3::operator/=(const Vector3& other)
{
    m_x /= other.m_x;
    m_y /= other.m_y;
    m_z /= other.m_z;
    return *this;
}

Vector3 Vector3::operator*(const Vector3::IMPL_TYPE other) const
{
    return Vector3(m_x * other, m_y * other, m_z * other);
}

Vector3 Vector3::operator/(const Vector3::IMPL_TYPE other) const
{
    IMPL_TYPE inv = 1.0 / other;
    return Vector3(m_x * inv, m_y * inv, m_z * inv);
}

Vector3& Vector3::operator*=(const Vector3::IMPL_TYPE other)
{
    m_x *= other;
    m_y *= other;
    m_z *= other;
    return *this;
}

Vector3& Vector3::operator/=(const Vector3::IMPL_TYPE other)
{
    IMPL_TYPE inv = 1.0 / other;
    m_x *= inv;
    m_y *= inv;
    m_z *= inv;
    return *this;
}

bool Vector3::HasNaNs() const
{
    return std::isnan(m_x) || std::isnan(m_y) || std::isnan(m_z);
}

bool Vector3::operator==(const Vector3& other) const
{
    return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
}

bool Vector3::operator!=(const Vector3& other) const
{
    return m_x != other.m_x || m_y != other.m_y || m_z != other.m_z;
}
