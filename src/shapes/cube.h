




#ifndef BSSRDF_CUBE_H
#define BSSRDF_CUBE_H

#include "shape.h"


class Cube : public Shape
{
public:
   Cube(const Vector& position, const Material& material, float edge);

   virtual ~Cube() = default;

   virtual Intersection intersects(const Ray& ray) const;
   virtual Vector getNormal(const Vector& position) const;

protected:
   const float _a; 	// size of the edge 
   Vector _bounds[2];
};


#endif //  BSSRDF_CUBE_H
