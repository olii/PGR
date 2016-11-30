#ifndef BSSRDF_RAY_H
#define BSSRDF_RAY_H

#include <glm/glm.hpp>

class Ray
{
public:
    Ray(const glm::dvec3& origin, const glm::dvec3& direction);

    const glm::dvec3& getOrigin() const;
    const glm::dvec3& getDirection() const;

    glm::dvec3 getPoint(double t) const;

private:
    glm::dvec3 _origin, _direction;
};

#endif
