#ifndef BSSRDF_RAY_TRACER_H
#define BSSRDF_RAY_TRACER_H

class Camera;
class Scene;

class RayTracer
{
public:
	void raytrace(const Camera& camera, const Scene& scene) const;
};

#endif
