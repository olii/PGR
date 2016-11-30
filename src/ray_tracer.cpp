#include "camera.h"
#include "intersection.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"

void RayTracer::raytrace(const Camera& camera, const Scene& scene) const
{
    auto& screen = camera.getScreen();

    for (std::uint32_t y = 0; y < screen.getHeight(); ++y)
    {
        for (std::uint32_t x = 0; x < screen.getWidth(); ++x)
        {
            auto ray = camera.getRay(x, y);
            auto hit = _castRayToScene(ray, scene);

            if (hit)
            {
                auto object = hit.getObject();
                auto normal = object->getNormal(hit.getPosition());
                auto color = object->getMaterial().getColor() * std::max(glm::dot(normal, -ray.getDirection()), 0.0);
                screen.putPixel(x, y, color);
            }
            else
                screen.putPixel(x, y, screen.getBackgroundColor());
        }
    }
}

Intersection RayTracer::_castRayToScene(const Ray& ray, const Scene& scene) const
{
    Intersection nearestHit;

    for (auto&& object : scene)
    {
        auto intersection = object->intersects(ray);
        if (intersection.hit() && intersection.getDistance() < nearestHit.getDistance())
            nearestHit = intersection;
    }

    return nearestHit;
}
