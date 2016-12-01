#include "intersection.h"
#include "ray.h"
#include "scene.h"

Scene::Scene(const Camera& camera) : _camera(camera), _lights(), _objects()
{
}

void Scene::addObject(std::unique_ptr<Shape>&& object)
{
    _objects.push_back(std::move(object));
}

void Scene::addLight(std::unique_ptr<Light>&& light)
{
    _lights.push_back(std::move(light));
}

Ray Scene::getRay(double x, double y) const
{
    return _camera.getRay(x, y);
}

Ray Scene::getRayAA(double x, double y) const
{
    return _camera.getRayAA(x, y);
}

Intersection Scene::castRay(const Ray& ray) const
{
    return castRay(ray, [](auto) { return true; });
}

Intersection Scene::castRay(const Ray& ray, const std::function<bool(const Intersection&)>& pred) const
{
    Intersection nearestHit;

    for (auto&& object : *this)
    {
        auto intersection = object->intersects(ray);
        if (intersection.hit() && intersection.getDistance() < nearestHit.getDistance() && pred(intersection))
            nearestHit = intersection;
    }

    return nearestHit;
}

Camera& Scene::getCamera()
{
    return _camera;
}

const Camera& Scene::getCamera() const
{
    return _camera;
}

const Scene::Lights& Scene::getLights() const
{
    return _lights;
}
