#ifndef PLANE_H
#define PLANE_H

#include "shape.h"

class Plane : public Shape
{
public:
    Plane(const Vector& position, const Vector& normal, double edge, const std::shared_ptr<Material>& material);
    Plane(const Vector& position, const Vector& normal, double edge, std::shared_ptr<Material>&& material);

    virtual Intersection intersects(const Ray& ray) const override;
    virtual Vector getNormal(const Vector&) const override;

private:
    Vector _normal;
    double _edge;
};

#endif   // PLANE_H
