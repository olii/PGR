#include "light.h"

Light::Light(const glm::dvec3& position, const Color& color) : _position(position), _color(color)
{
}

const glm::dvec3& Light::getPosition() const
{
    return _position;
}

const Color& Light::getColor() const
{
    return _color;
}
