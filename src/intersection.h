#ifndef BSSRDF_INTERSECTION_H
#define BSSRDF_INTERSECTION_H

#include "vector.h"

class Shape;

class Intersection
{
public:
    Intersection();
    Intersection(double distance, const Vector& position, const Vector& rayDirection, const Shape* object);

    operator bool() const;

    bool hit() const;
    bool miss() const;

    double getDistance() const;
    const Vector& getPosition() const;
    const Vector& getRayDirection() const;
    const Shape* getObject() const;

private:
    double _distance;
    Vector _position;
    Vector _rayDirection;
    const Shape* _object;
};

#endif
