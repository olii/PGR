#ifndef BSSRDF_SHAPES_SPHERE_H
#define BSSRDF_SHAPES_SPHERE_H

#include "shape.h"

class Sphere : public Shape
{
public:
    Sphere(const glm::dvec3& position, double radius, const Material& material);

    double getRadius() const;

    virtual bool intersects(const Ray& ray, double& hit) const override;

private:
    double _radius;
};

#endif
