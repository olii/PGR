#include "scene.h"

Scene::Scene(const Light& light) : _light(light)
{
}

void Scene::addObject(std::unique_ptr<Shape>&& object)
{
    _objects.push_back(std::move(object));
}

const Light& Scene::getLight() const
{
    return _light;
}
