#include "light.h"

Light::Light(const Vector& position, const Color& color) : _position(position), _color(color)
{
}

const Vector& Light::getPosition() const
{
    return _position;
}

const Color& Light::getColor() const
{
    return _color;
}
