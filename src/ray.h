#ifndef BSSRDF_RAY_H
#define BSSRDF_RAY_H

#include "vector.h"

class Ray
{
public:
    Ray(const Vector& origin, const Vector& direction);

    const Vector& getOrigin() const;
    const Vector& getDirection() const;

    Vector getPoint(double t) const;

private:
    Vector _origin, _direction;
};

#endif
