#ifndef BSSRDF_MATERIALS_BRDF_MATERIAL_H
#define BSSRDF_MATERIALS_BRDF_MATERIAL_H

#include "material.h"

class BrdfMaterial : public Material
{
public:
    BrdfMaterial(const Color& color, double diffuseCoeff, double specularCoeff);

    double getDiffuseCoeff() const;
    double getSpecularCoeff() const;

    virtual Color calculateColor(const Intersection& hit, const Scene& scene, const std::vector<const Light*>& visibleLights) const override;

private:
    double _diffuseCoeff;
    double _specularCoeff;
};

#endif
