#include <glm/gtx/norm.hpp>

#include "camera.h"
#include "intersection.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"

namespace {
constexpr bool AA_ENABLE = false;
constexpr int AA_SAMPLES = 25;
}

void RayTracer::raytrace(const Scene& scene) const
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
                    auto visibleLights = _castShadowRays(hit, scene);
                    color += hit.getObject()->getMaterial()->calculateColor(hit, scene, visibleLights);
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

std::vector<const Light*> RayTracer::_castShadowRays(const Intersection& hitPoint, const Scene& scene) const
{
    std::vector<const Light*> result;
    result.reserve(scene.getLights().size());

    for (const auto& light : scene.getLights())
    {
        auto lightDir = light->getPosition() - hitPoint.getPosition();
        auto point = hitPoint.getPosition() + 0.01 * hitPoint.getObject()->getNormal(hitPoint.getPosition());
        Ray shadowRay(point, lightDir);

        // Shadow rays may cause "shadow acne" because these rays will start hitting the object itself in almost zero distance
        // Let's just check whether the hit object is the same as our source object and check the distance against threshold
        auto hit = scene.castRay(shadowRay);

        // We did not hit any object or we hit object farther away than light
        if (!hit || glm::length2(hit.getPosition() - hitPoint.getPosition()) > glm::length2(lightDir))
            result.push_back(light.get());
    }

    return result;
}
