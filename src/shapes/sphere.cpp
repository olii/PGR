#include <glm/gtx/norm.hpp>

#include "ray.h"
#include "shapes/sphere.h"

Sphere::Sphere(const glm::dvec3& position, double radius, const Material& material) : Shape(position, material), _radius(radius)
{
}

double Sphere::getRadius() const
{
	return _radius;
}

bool Sphere::intersects(const Ray& ray, double& hit) const
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
		{
			hit = solution;
			return true;
		}
		solution = (-b + sqrtD) / (2.0 * a);
		if (solution > 0.0)
		{
			hit = solution;
			return true;
		}
	}

	hit = NAN;
	return false;
}
