#include "materials/bssrdf_material.h"

BssrdfMaterial::BssrdfMaterial(const Color& color, double absorbCoeff, double scatterCoeff)
    : Material(color), _absorbCoeff(absorbCoeff), _scatterCoeff(scatterCoeff)
{
}

double BssrdfMaterial::getAbsorbCoeff() const
{
    return _absorbCoeff;
}

double BssrdfMaterial::getScatterCoeff() const
{
    return _scatterCoeff;
}

Color BssrdfMaterial::calculateColor(const Intersection&, const Scene&, const std::vector<const Light*>&) const
{
    return _color;
}
