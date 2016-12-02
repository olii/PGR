#include <glm/gtx/norm.hpp>

#include "intersection.h"
#include "ray.h"
#include "shapes/sphere.h"

Sphere::Sphere(const Vector& position, double radius, const std::shared_ptr<Material>& material)
    : Shape(position, material), _radius(radius)
{
}

Sphere::Sphere(const Vector& position, double radius, std::shared_ptr<Material>&& material)
    : Shape(position, std::move(material)), _radius(radius)
{
}

double Sphere::getRadius() const
{
    return _radius;
}

Intersection Sphere::intersects(const Ray& ray) const
{
    const auto& origin = ray.getOrigin();
    const auto& direction = ray.getDirection();

    // Direction of ray is always unit-vector thus we don't need a
    auto a = glm::dot(direction, direction);
    auto b = 2.0 * glm::dot(direction, (origin - _position));
    auto c = glm::dot(origin - _position, origin - _position) - _radius * _radius;

    auto D = b * b - 4.0 * a * c;
    if (D > 0.0)
    {
        auto sqrtD = std::sqrt(D);
        auto solution = (-b - sqrtD) / (2.0 * a);
        if (solution > 0.0)
            return {solution, ray.getPoint(solution), this};

        solution = (-b + sqrtD) / (2.0 * a);
        if (solution > 0.0)
            return {solution, ray.getPoint(solution), this};
    }

    return {};
}

Vector Sphere::getNormal(const Vector& position) const
{
    return glm::normalize(position - getPosition());
}
