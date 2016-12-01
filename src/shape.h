#ifndef BSSRDF_SHAPE_H
#define BSSRDF_SHAPE_H

#include "material.h"
#include "vector.h"

class Intersection;
class Ray;

class Shape
{
public:
    Shape(const Vector& position, const Material& material) : _position(position), _material(material) {}
    virtual ~Shape() = default;

    const Vector& getPosition() const { return _position; }
    const Material& getMaterial() const { return _material; }

    virtual Intersection intersects(const Ray& ray) const = 0;
    virtual Vector getNormal(const Vector& position) const = 0;

protected:
    Vector _position;
    Material _material;
};

#endif
