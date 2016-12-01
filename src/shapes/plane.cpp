#include "plane.h"
#include "intersection.h"
#include "ray.h"

Plane::Plane(const Vector& position, const Vector& normal, const Material& material)
  : Shape(position, material), _normal(glm::normalize(normal))
{
}

Intersection Plane::intersects(const Ray& ray) const
{
    float denom = glm::dot(_normal, ray.getDirection());
    if (std::abs(denom) > 1e-6)
    {
        float t = glm::dot(_position - ray.getOrigin(), _normal) / denom;
        if (t > 0)
        {
            return {t, ray.getPoint(t), this};
        }
    }
    return {};
}

Vector Plane::getNormal(const Vector& /*position*/) const
{
    return _normal;
}
