#ifndef BSSRDF_RAY_TRACER_H
#define BSSRDF_RAY_TRACER_H

#include <functional>
#include <vector>

#include "light.h"
#include "vector.h"

class Intersection;
class Ray;
class Scene;

class RayTracer
{
public:
    void raytrace(const Scene& scene) const;

private:
    std::vector<const Light*> _castShadowRays(const Intersection& hitPoint, const Scene& scene) const;
};

#endif
