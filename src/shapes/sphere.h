#ifndef BSSRDF_SHAPES_SPHERE_H
#define BSSRDF_SHAPES_SPHERE_H

#include "shape.h"

class Sphere : public Shape
{
public:
    Sphere(const Vector& position, double radius, const Material& material);

    double getRadius() const;

    virtual Intersection intersects(const Ray& ray) const override;
    virtual Vector getNormal(const Vector& position) const override;

private:
    double _radius;
};

#endif
