#ifndef BSSRDF_MATERIAL_H
#define BSSRDF_MATERIAL_H

#include <vector>

#include "color.h"
#include "light.h"

class Intersection;
class Scene;

class Material
{
public:
    Material(const Color& color) : _color(color) {}

    virtual ~Material() = default;

    const Color& getColor() const { return _color; }

    virtual Color calculateColor(const Intersection& hit, const Scene& scene, const std::vector<const Light*>& visibleLights) const = 0;

protected:
    Color _color;
};

#endif
