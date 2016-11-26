#include "ray.h"

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction) : _origin(origin), _direction(direction)
{
}

const glm::dvec3& Ray::getOrigin() const
{
	return _origin;
}

const glm::dvec3& Ray::getDirection() const
{
	return _direction;
}

glm::dvec3 Ray::getPoint(double t) const
{
	return _origin + t * _direction;
}
