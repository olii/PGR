#ifndef BSSRDF_MATERIALS_BSSRDF_MATERIAL_H
#define BSSRDF_MATERIALS_BSSRDF_MATERIAL_H

#include "material.h"

class BssrdfMaterial : public Material
{
public:
    BssrdfMaterial(const Color& color, double absorbCoeff, double scatterCoeff);

    double getAbsorbCoeff() const;
    double getScatterCoeff() const;

    virtual Color calculateColor(const Intersection& hit, const Scene& scene, const std::vector<const Light*>& visibleLights) const override;

private:
    double _absorbCoeff;
    double _scatterCoeff;
};

#endif
