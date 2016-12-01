#include "ray.h"

Ray::Ray(const Vector& origin, const Vector& direction) : _origin(origin), _direction(glm::normalize(direction))
{
}

const Vector& Ray::getOrigin() const
{
    return _origin;
}

const Vector& Ray::getDirection() const
{
    return _direction;
}

Vector Ray::getPoint(double t) const
{
    return _origin + t * _direction;
}
