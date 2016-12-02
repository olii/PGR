#ifndef BSSRDF_CUBE_H
#define BSSRDF_CUBE_H

#include <array>

#include "shape.h"

class Cube : public Shape
{
public:
   Cube(const Vector& position, const Material& material, double edge);

   virtual Intersection intersects(const Ray& ray) const override;
   virtual Vector getNormal(const Vector& position) const override;

protected:
   double _edge; // size of the edge
   std::array<Vector, 2> _bounds;
};


#endif //  BSSRDF_CUBE_H
