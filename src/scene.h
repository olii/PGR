#ifndef BSSRDF_SCENE_H
#define BSSRDF_SCENE_H

#include <functional>
#include <memory>
#include <vector>

#include "camera.h"
#include "light.h"
#include "shape.h"

class Intersection;
class Ray;

class Scene
{
public:
    using Lights = std::vector<std::unique_ptr<Light>>;
    using Objects = std::vector<std::unique_ptr<Shape>>;

    Scene(const Camera& camera);

    void addObject(std::unique_ptr<Shape>&& object);
    void addLight(std::unique_ptr<Light>&& light);

    Ray getRay(double x, double y) const;
    Ray getRayAA(double x, double y) const;

    Intersection castRay(const Ray& ray) const;
    Intersection castRay(const Ray& ray, const std::function<bool(const Intersection&)>& pred) const;
    bool castShadowRay(const Vector& position, const Shape* object, const Light* light) const;
    std::vector<const Light*> castShadowRays(const Vector& position, const Shape* object) const;

    Camera& getCamera();
    const Camera& getCamera() const;
    const Lights& getLights() const;

    auto begin() { return _objects.begin(); }
    auto end() { return _objects.end(); }
    auto begin() const { return _objects.begin(); }
    auto end() const { return _objects.end(); }

private:
    Camera _camera;
    Lights _lights;
    Objects _objects;
};

#endif
