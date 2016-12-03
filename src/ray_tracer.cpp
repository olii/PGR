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
                    auto visibleLights = scene.castShadowRays(hit.getPosition(), hit.getObject());
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
