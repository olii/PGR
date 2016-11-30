#include "camera.h"
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

            const Shape* nearestObject = nullptr;
            double hit;
            double nearestHit = std::numeric_limits<double>::max();

            for (auto&& object : scene)
            {
                if (object->intersects(ray, hit) && hit < nearestHit)
                {
                    nearestObject = object.get();
                    nearestHit = hit;
                }
            }

            if (nearestObject)
            {
                auto p = ray.getPoint(hit);
                auto normal = nearestObject->getNormal(p);
                auto f = glm::dot(-normal, glm::normalize(ray.getDirection()));
                auto color = nearestObject->getMaterial().getColor() * f;
                screen.putPixel(x, y, color);
            }
            else
                screen.putPixel(x, y, screen.getBackgroundColor());
        }
    }
}
