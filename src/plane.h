#ifndef PLANE_H
#define PLANE_H

#include "objectgeometry.h"
#include "point.h"
#include "ray.h"
#include <experimental/optional>

using std::experimental::optional;

class Plane : public ObjectGeometry
{
public:
    Plane(const Point& p, const Vector3& normal) : m_p(p), m_normal(normal.GetNormalized()) {}
    optional<float> Intersect(const Ray& ray);

private:
    Point m_p;
    Vector3 m_normal;
};

#endif   // PLANE_H
