#ifndef BSSRDF_LIGHT_MODEL_H
#define BSSRDF_LIGHT_MODEL_H

#include <vector>

#include "color.h"
#include "vector.h"

class Intersection;
class Scene;

class LightModel
{
public:
    using LightVisibilities = std::vector<Vector>;

    virtual ~LightModel() = default;

    virtual Color calculateColor(const Intersection& intersection, const Scene& scene, const LightVisibilities& lightVisibilities) = 0;
};

#endif
