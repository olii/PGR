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

                double diffuseTerm = 0.0;
                auto shadowRayDirs = _castShadowRays(hit, scene);
                for (const auto& shadowRayDir : shadowRayDirs)
                {
                    if (!std::isnan(shadowRayDir.x))
                        diffuseTerm = std::min(diffuseTerm + std::max(glm::dot(normal, shadowRayDir), 0.0), 1.0);
                }

                auto color = object->getMaterial().getColor() * diffuseTerm;
                screen.putPixel(x, y, color);
            }
            else
                screen.putPixel(x, y, screen.getBackgroundColor());
        }
    }
}

Intersection RayTracer::_castRayToScene(const Ray& ray, const Scene& scene) const
{
    return _castRayToScene(ray, scene, [](auto) { return true; });
}

Intersection RayTracer::_castRayToScene(const Ray& ray, const Scene& scene, const std::function<bool(const Intersection&)>& pred) const
{
    Intersection nearestHit;

    for (auto&& object : scene)
    {
        auto intersection = object->intersects(ray);
        if (intersection.hit() && intersection.getDistance() < nearestHit.getDistance() && pred(intersection))
            nearestHit = intersection;
    }

    return nearestHit;
}

std::vector<Vector> RayTracer::_castShadowRays(const Intersection& hitPoint, const Scene& scene) const
{
    std::vector<Vector> result;
    result.reserve(scene.getLights().size());

    for (const auto& light : scene.getLights())
    {
        Ray shadowRay(hitPoint.getPosition(), light->getPosition() - hitPoint.getPosition());

        // Shadow rays may cause "shadow acne" because these rays will start hitting the object itself in almost zero distance
        // Let's just check whether the hit object is the same as our source object and check the distance against threshold
        auto hit = _castRayToScene(shadowRay, scene, [&hitPoint](auto hit)
                {
                    return hit.getObject() != hitPoint.getObject() || hit.getDistance() > 0.01;
                });

        // We did not hit any object and since ray is in light's direction it must have hit the light
        if (!hit)
            result.push_back(shadowRay.getDirection());
        else
            result.push_back({NAN, NAN, NAN});
    }

    return result;
}
