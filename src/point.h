#ifndef POINT_H
#define POINT_H

#include "vector3.h"

class Point
{
public:
    Point(float x, float y, float z) : m_x(x), m_y(y), m_z(z){};
    Point(int x, int y, int z) : m_x(x), m_y(y), m_z(z){};

    Point operator+(const Vector3& vector) const
    {
        return Point(m_x + vector.GetX(), m_y + vector.GetY(), m_z + vector.GetZ());
    }
    Point& operator+=(const Vector3& vector)
    {
        m_x += vector.GetX();
        m_y += vector.GetY();
        m_z += vector.GetZ();
    }
    Vector3 operator-(const Point& other) const
    {
        return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
    }
    inline float GetX() const { return m_x; }
    inline float GetY() const { return m_y; }
    inline float GetZ() const { return m_z; }

    inline void SetX(float x) { m_x = x; }
    inline void SetY(float y) { m_y = y; }
    inline void SetZ(float z) { m_z = z; }

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif   // POINT_H
