#ifndef BSSRDF_RAY_TRACER_H
#define BSSRDF_RAY_TRACER_H

#include <functional>
#include <vector>

#include "vector.h"

class Camera;
class Intersection;
class Ray;
class Scene;

class RayTracer
{
public:
    void raytrace(const Camera& camera, const Scene& scene) const;

private:
    Intersection _castRayToScene(const Ray& ray, const Scene& scene) const;
    Intersection _castRayToScene(const Ray& ray, const Scene& scene, const std::function<bool(const Intersection&)>& pred) const;
    std::vector<Vector> _castShadowRays(const Intersection& hitPoint, const Scene& scene) const;
};

#endif
