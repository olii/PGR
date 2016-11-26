#include "scene.h"

void Scene::addObject(std::unique_ptr<Shape>&& object)
{
	_objects.push_back(std::move(object));
}
