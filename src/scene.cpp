#include "scene.h"

Scene::Scene()
{
}

void Scene::addObject(std::unique_ptr<Shape>&& object)
{
    _objects.push_back(std::move(object));
}

void Scene::addLight(std::unique_ptr<Light>&& light)
{
    _lights.push_back(std::move(light));
}

const Scene::Lights& Scene::getLights() const
{
    return _lights;
}
