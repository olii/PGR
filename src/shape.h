#ifndef BSSRDF_SHAPE_H
#define BSSRDF_SHAPE_H

#include <memory>

#include "material.h"
#include "vector.h"

class Intersection;
class Ray;

class Shape
{
public:
    Shape(const Vector& position, const std::shared_ptr<Material>& material) : _position(position), _material(material) {}
    Shape(const Vector& position, std::shared_ptr<Material>&& material) : _position(position), _material(std::move(material)) {}
    virtual ~Shape() = default;

    const Vector& getPosition() const { return _position; }
    const Material* getMaterial() const { return _material.get(); }

    virtual Intersection intersects(const Ray& ray) const = 0;
    virtual Vector getNormal(const Vector& position) const = 0;

protected:
    Vector _position;
    std::shared_ptr<Material> _material;
};

#endif
