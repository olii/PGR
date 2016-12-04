#include "intersection.h"

Intersection::Intersection() : _distance(std::numeric_limits<double>::max()), _position(), _object(nullptr)
{
}

Intersection::Intersection(double distance, const Vector& position, const Vector& rayDirection, const Shape* object)
    : _distance(distance), _position(position), _rayDirection(rayDirection), _object(object)
{
}

Intersection::operator bool() const
{
    return hit();
}

bool Intersection::hit() const
{
    return _object != nullptr;
}

bool Intersection::miss() const
{
    return !hit();
}

double Intersection::getDistance() const
{
    return _distance;
}

const Vector& Intersection::getPosition() const
{
    return _position;
}

const Vector& Intersection::getRayDirection() const
{
    return _rayDirection;
}

const Shape* Intersection::getObject() const
{
    return _object;
}
