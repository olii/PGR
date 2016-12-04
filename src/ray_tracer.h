#ifndef BSSRDF_RAY_TRACER_H
#define BSSRDF_RAY_TRACER_H

#include <functional>
#include <vector>

#include "light.h"
#include "multithreading/thread_pool.h"
#include "vector.h"

class Intersection;
class Ray;
class Scene;

class RayTracer
{
public:
    using Task = std::tuple<std::uint32_t, std::uint32_t, const Scene*>;
    using Result = std::tuple<std::uint32_t, std::uint32_t, Color>;

    RayTracer(std::size_t numOfThreads);

    void raytrace(const Scene& scene);
    void stop();

private:
    static Result _raytracePixel(Task task);

    ThreadPool<Task, Result> _threadPool;
};

#endif
