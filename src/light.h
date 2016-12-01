#ifndef BSSRDF_LIGHT_H
#define BSSRDF_LIGHT_H

#include "color.h"
#include "vector.h"

class Light
{
public:
    Light(const Vector& position, const Color& color);

    const Vector& getPosition() const;
    const Color& getColor() const;

private:
    Vector _position;
    Color _color;
};

#endif
