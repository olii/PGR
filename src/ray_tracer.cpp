#include "camera.h"
#include "intersection.h"
#include "light_model.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"

namespace {
constexpr bool AA_ENABLE = false;
constexpr int AA_SAMPLES = 25;
}

void RayTracer::raytrace(const Scene& scene, LightModel* lightModel) const
{
    auto& screen = scene.getCamera().getScreen();

    for (std::uint32_t y = 0; y < screen.getHeight(); ++y)
    {
        for (std::uint32_t x = 0; x < screen.getWidth(); ++x)
        {
            Color color;
            for (int samples = 0; samples < (AA_ENABLE?AA_SAMPLES:1); samples++)
            {
                auto ray = AA_ENABLE ? scene.getRayAA(x, y) : scene.getRay(x, y);
                auto hit = scene.castRay(ray);

                if (hit)
                {
                    auto shadowRayDirs = _castShadowRays(hit, scene);
                    color += lightModel->calculateColor(hit, scene, shadowRayDirs);
                }
                else
                {
                    color += screen.getBackgroundColor();
                }
            }
            if (AA_ENABLE)
                color /= static_cast<double>(AA_SAMPLES);
            screen.putPixel(x, y, color);
        }
    }
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
        auto hit = scene.castRay(shadowRay, [&hitPoint](auto hit) {
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
