#ifndef BSSRDF_RAY_TRACER_H
#define BSSRDF_RAY_TRACER_H

#include <functional>
#include <vector>

#include "vector.h"

class Intersection;
class LightModel;
class Ray;
class Scene;

class RayTracer
{
public:
    void raytrace(const Scene& scene, LightModel* lightModel) const;

private:
    std::vector<Vector> _castShadowRays(const Intersection& hitPoint, const Scene& scene) const;
};

#endif
