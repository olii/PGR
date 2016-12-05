#include "plane.h"
#include "intersection.h"
#include "ray.h"

Plane::Plane(const Vector& position, const Vector& normal, double edge, const std::shared_ptr<Material>& material)
    : Shape(position, material), _normal(glm::normalize(normal)), _edge(edge)
{
}

Plane::Plane(const Vector& position, const Vector& normal, double edge, std::shared_ptr<Material>&& material)
    : Shape(position, std::move(material)), _normal(glm::normalize(normal)), _edge(edge)
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
            auto p = ray.getPoint(t);
            if (std::abs(p.x - _position.x) < _edge && std::abs(p.y - _position.y) < _edge && std::abs(p.z - _position.z) < _edge)
                return {t, p, ray.getDirection(), this};
            else
                return {};
        }
    }
    return {};
}

Vector Plane::getNormal(const Vector& /*position*/) const
{
    return _normal;
}
