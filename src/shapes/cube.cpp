
#include "cube.h"
#include "intersection.h"
#include "ray.h" 
#include <algorithm>

using FLOAT = float;

Cube::Cube(const Vector& pos, const Material& material, float edge) : 
   Shape(pos, material), _a(edge) 
{
   const float edge2 = edge / 2.0f;
   const Vector p0 = {pos.x-edge2, pos.y-edge2, pos.z-edge2}; // lower front left
   const Vector p1 = {pos.x+edge2, pos.y+edge2, pos.z+edge2}; // upper back right

   _bounds[0] = p0;
   _bounds[1] = p1;
}

/*
 * Compute intersection with cube
 * Inspired by https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection 
 */
Intersection Cube::intersects(const Ray& ray) const
{
   FLOAT t0x, t1x, t0y, t1y, t0z, t1z, Dx, Dy, Dz, Ox, Oy, Oz, x, y, z;
   Dx = ray.getDirection().x;
   Dy = ray.getDirection().y;
   Dz = ray.getDirection().z;

   Ox = ray.getOrigin().x;
   Oy = ray.getOrigin().y;
   Oz = ray.getOrigin().z;

   FLOAT tmin = (_bounds[0].x - Ox) / Dx; 
   FLOAT tmax = (_bounds[1].x - Ox) / Dx; 
 
   if (tmin > tmax) 
      std::swap(tmin, tmax);
 
   FLOAT tymin = (_bounds[0].y - Oy) / Dy; 
   FLOAT tymax = (_bounds[1].y - Oy) / Dy; 
 
   if (tymin > tymax) 
      std::swap(tymin, tymax); 
 
   if ((tmin > tymax) || (tymin > tmax)) 
       return Intersection(); // no hit
 
   if (tymin > tmin) 
       tmin = tymin; 
 
   if (tymax < tmax) 
       tmax = tymax; 
 
   FLOAT tzmin = (_bounds[0].z - Oz) / Dz; 
   FLOAT tzmax = (_bounds[1].z - Oz) / Dz; 

   if (tzmin > tzmax) 
      std::swap(tzmin, tzmax); 
 
   if ((tmin > tzmax) || (tzmin > tmax)) 
      return Intersection(); // no hit
 
   if (tzmin > tmin) 
      tmin = tzmin; 
 
   if (tzmax < tmax) 
      tmax = tzmax; 

   // return first hit (tmin)
   return Intersection(tmin, ray.getPoint(tmin), this);
}

Vector Cube::getNormal(const Vector& position) const
{
   const FLOAT tresh = 1e-6;
   Vector res(0,0,0);
  
   FLOAT d0x, d1x, d0y, d1y, d0z, d1z;

   d0x = position.x - _bounds[0].x;
   d1x = position.x - _bounds[1].x;

   d0y = position.y - _bounds[0].y;
   d1y = position.y - _bounds[1].y;

   d0z = position.z - _bounds[0].z;
   d1z = position.z - _bounds[1].z;

   bool xcheck = (position.x > (_bounds[0].x - tresh)) && (position.x < _bounds[1].x + tresh);
   bool ycheck = (position.y > (_bounds[0].y - tresh)) && (position.y < _bounds[1].y + tresh);
   bool zcheck = (position.z > (_bounds[0].z - tresh)) && (position.z < _bounds[1].z + tresh);

   const FLOAT s = 1.0; // size of returned normal vector

   if (d0x < tresh && ycheck && zcheck)      // left
      res = {-s, 0, 0};

   else if (d0z < tresh && ycheck && xcheck) // front
      res = {0,  0, -s};

   else if (d1x < tresh && ycheck && zcheck) // right
      res = {s,  0, 0};

   else if (d1z < tresh && ycheck && xcheck) // back
      res = {0,  0, s};

   else if (d1y < tresh && xcheck && zcheck) // top
      res = {0,  s, 0};

   else if (d0y < tresh && xcheck && zcheck) // bottom
      res = {0, -s, 0};

   return res;
}






