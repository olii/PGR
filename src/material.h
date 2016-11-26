#ifndef BSSRDF_MATERIAL_H
#define BSSRDF_MATERIAL_H

#include "color.h"

class Material
{
public:
	Material(const Color& color);

	const Color& getColor() const;

private:
	Color _color;
};

#endif
