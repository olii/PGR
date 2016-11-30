#ifndef BSSRDF_SCENE_H
#define BSSRDF_SCENE_H

#include <memory>
#include <vector>

#include "light.h"
#include "shape.h"

class Scene
{
public:
    Scene(const Light& light);

    void addObject(std::unique_ptr<Shape>&& object);

    const Light& getLight() const;

    auto begin() { return _objects.begin(); }
    auto end() { return _objects.end(); }
    auto begin() const { return _objects.begin(); }
    auto end() const { return _objects.end(); }

private:
    Light _light;
    std::vector<std::unique_ptr<Shape>> _objects;
};

#endif
