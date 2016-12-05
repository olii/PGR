#include <functional>
#include <iostream>

#include "camera.h"
#include "intersection.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"
#include "settings.h"

namespace {

constexpr bool AA_ENABLE = false;
constexpr int AA_SAMPLES = 25;

}

RayTracer::RayTracer(std::size_t numOfThreads) : _threadPool(numOfThreads, std::bind(&_raytracePixel, std::placeholders::_1))
{
    _threadPool.start();
}

void RayTracer::raytrace(Scene* scene)
{
    auto& screen = scene->getCamera().getScreen();

    for (std::uint32_t y = 0; y < screen.getHeight(); ++y)
    {
        for (std::uint32_t x = 0; x < screen.getWidth(); ++x)
        {
            _threadPool.enqueueTask(std::make_tuple(x, y, scene));
        }
    }

    std::size_t expected = screen.getWidth() * screen.getHeight();
    for (std::size_t i = 0; i < expected; ++i)
    {
        auto result = _threadPool.dequeueResult();
        std::uint32_t x = std::get<0>(result);
        std::uint32_t y = std::get<1>(result);
        Color color = std::get<2>(result);

        screen.putPixel(x, y, color);
    }
}

void RayTracer::stop()
{
    _threadPool.terminate();
}

RayTracer::Result RayTracer::_raytracePixel(Task task)
{
    std::uint32_t x = std::get<0>(task);
    std::uint32_t y = std::get<1>(task);
    const Scene* scene = std::get<2>(task);

    static std::size_t numSamples = Settings::instance().isAAEnabled() ? Settings::instance().getAASamplesCount() : 1;
    static auto getRay = Settings::instance().isAAEnabled() ?
        std::bind(&Scene::getRayAA, scene, std::placeholders::_1, std::placeholders::_2) :
        std::bind(&Scene::getRay, scene, std::placeholders::_1, std::placeholders::_2);

    Color color;
    for (std::size_t samples = 0; samples < numSamples; samples++)
    {
        auto ray = getRay(x, y);
        auto hit = scene->castRay(ray);

        if (hit)
        {
            auto visibleLights = scene->castShadowRays(hit.getPosition(), hit.getObject());
            color += hit.getObject()->getMaterial()->calculateColor(hit, *scene, visibleLights);
        }
        else
        {
            color += scene->getCamera().getScreen().getBackgroundColor();
        }
    }

    color /= static_cast<double>(numSamples);
    return std::make_tuple(x, y, color);
}
