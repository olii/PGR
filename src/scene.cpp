#include <glm/gtx/norm.hpp>

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

bool Scene::castShadowRay(const Vector& position, const Shape* object, const Light* light) const
{
    auto lightDir = light->getPosition() - position;

    // Shadow rays may cause "shadow acne" because these rays will start hitting the object itself in almost zero distance
    // We will just start the ray in a very small distance from the actual point in direction of normal
    auto point = position + 0.01 * object->getNormal(position);
    Ray shadowRay(point, lightDir);

    auto hit = castRay(shadowRay);

    // We did not hit any object or we hit object farther away than light
    if (!hit || glm::length2(hit.getPosition() - position) > glm::length2(lightDir))
        return true;

    return false;
}

std::vector<const Light*> Scene::castShadowRays(const Vector& position, const Shape* object) const
{
    std::vector<const Light*> result;
    result.reserve(_lights.size());

    for (const auto& light : _lights)
    {
        if (castShadowRay(position, object, light.get()))
            result.push_back(light.get());
    }

    return result;
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
