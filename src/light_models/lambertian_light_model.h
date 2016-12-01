#ifndef BSSRDF_LIGHT_MODELS_LAMBERTIAN_LIGHT_MODEL_H
#define BSSRDF_LIGHT_MODELS_LAMBERTIAN_LIGHT_MODEL_H

#include "light_model.h"

class LambertianLightModel : public LightModel
{
public:
    virtual Color calculateColor(const Intersection& hit, const Scene& scene, const LightVisibilities& lightVisibilities) override;
};

#endif
