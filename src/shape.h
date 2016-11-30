#ifndef BSSRDF_SHAPE_H
#define BSSRDF_SHAPE_H

#include <glm/glm.hpp>

#include "material.h"

class Ray;

class Shape
{
public:
    Shape(const glm::dvec3& position, const Material& material) : _position(position), _material(material) {}
    virtual ~Shape() = default;

    const glm::dvec3& getPosition() const { return _position; }
    const Material& getMaterial() const { return _material; }

    virtual bool intersects(const Ray& ray, double& hit) const = 0;

protected:
    glm::dvec3 _position;
    Material _material;
};

#endif
