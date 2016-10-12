#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector3.h"

class Ray
{
public:
    Ray(const Point& p, const Vector3& vec) : m_point(p), m_dir(vec) {}
    Point operator()(float f) { return m_point + (m_dir * f); }

    const Point& GetOrigin() const { return m_point; }
    const Vector3& GetDirection() const { return m_dir; }

private:
    Point m_point;
    Vector3 m_dir;
};

#endif   // RAY_H
