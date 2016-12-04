#ifndef BSSRDF_MATERIALS_BSSRDF_MATERIAL_H
#define BSSRDF_MATERIALS_BSSRDF_MATERIAL_H

#include "material.h"
#include <random>

class BssrdfMaterial : public Material
{
public:
    BssrdfMaterial(const Color& color, const Color& absorbCoeff, const Color& scatterCoeff, double phase, double eta);

    const Color& getAbsorbCoeff() const;
    const Color& getScatterCoeff() const;

    virtual Color calculateColor(const Intersection& hit, const Scene& scene, const std::vector<const Light*>& visibleLights) const override;

private:
    Color _single(const Intersection& hit, const Scene& scene) const;
    Color _diffuse(const Intersection& hit, const Scene& scene) const;

    std::vector<std::pair<Vector, double>> _samplePoints(const Intersection& hit, const Scene& scene) const;
    double _FdrIntegralApprox(double eta);
    Color _Rd(double distance) const;
    double _Fresnel(double angle) const;

    Color _absorbCoeff;
    Color _scatterCoeff;
    double _phase;
    double _eta;

    Color _reducedScatteringCoeff;
    Color _reducedExtinctionCoeff;
    Color _reducedAlbedo;
    double _effectiveTransportCoeff;
    double _fresnelDiffuseReflectance;
    double _boundary;
    Color _positiveDipoleDistance;
    Color _negativeDipoleDistance;
};

#endif
