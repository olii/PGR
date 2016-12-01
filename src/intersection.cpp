#include "intersection.h"

Intersection::Intersection() : _distance(std::numeric_limits<double>::max()), _position(), _object(nullptr)
{
}

Intersection::Intersection(double distance, const Vector& position, const Shape* object) : _distance(distance), _position(position), _object(object)
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

double Intersection::getDistance() const
{
    return _distance;
}

const Vector& Intersection::getPosition() const
{
    return _position;
}

const Shape* Intersection::getObject() const
{
    return _object;
}
