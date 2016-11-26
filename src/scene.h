#ifndef BSSRDF_SCENE_H
#define BSSRDF_SCENE_H

#include <memory>
#include <vector>

#include "shape.h"

class Scene
{
public:
	void addObject(std::unique_ptr<Shape>&& object);

	auto begin() { return _objects.begin(); }
	auto end() { return _objects.end(); }
	auto begin() const { return _objects.begin(); }
	auto end() const { return _objects.end(); }

private:
	std::vector<std::unique_ptr<Shape>> _objects;
};

#endif