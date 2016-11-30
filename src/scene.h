#ifndef BSSRDF_SCENE_H
#define BSSRDF_SCENE_H

#include <memory>
#include <vector>

#include "light.h"
#include "shape.h"

class Scene
{
public:
    typedef std::vector<std::unique_ptr<Light>> Lights;
    typedef std::vector<std::unique_ptr<Shape>> Objects;

    Scene();

    void addObject(std::unique_ptr<Shape>&& object);
    void addLight(std::unique_ptr<Light>&& object);

    const Lights& getLights() const;

    auto begin() { return _objects.begin(); }
    auto end() { return _objects.end(); }
    auto begin() const { return _objects.begin(); }
    auto end() const { return _objects.end(); }

private:
    Lights _lights;
    Objects _objects;
};

#endif
