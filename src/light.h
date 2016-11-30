#ifndef BSSRDF_LIGHT_H
#define BSSRDF_LIGHT_H

#include <glm/glm.hpp>

#include "color.h"

class Light
{
public:
	Light(const glm::dvec3& position, const Color& color);

	const glm::dvec3& getPosition() const;
	const Color& getColor() const;

private:
	glm::dvec3 _position;
	Color _color;
};

#endif
