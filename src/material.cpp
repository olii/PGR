#include "material.h"

Material::Material(const Color& color) : _color(color)
{
}

const Color& Material::getColor() const
{
	return _color;
}

