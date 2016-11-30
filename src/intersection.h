#ifndef BSSRDF_INTERSECTION_H
#define BSSRDF_INTERSECTION_H

#include <glm/glm.hpp>

class Shape;

class Intersection
{
public:
    Intersection();
    Intersection(double distance, const glm::dvec3& position, const Shape* object);

    operator bool() const;

    bool hit() const;

    double getDistance() const;
    const glm::dvec3& getPosition() const;
    const Shape* getObject() const;

private:
    double _distance;
    glm::dvec3 _position;
    const Shape* _object;
};

#endif
