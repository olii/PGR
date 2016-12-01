#include "color.h"
#include "intersection.h"
#include "light_models/lambertian_light_model.h"
#include "scene.h"

Color LambertianLightModel::calculateColor(const Intersection& hit, const Scene&, const LightVisibilities& lightVisibilities)
{
    auto object = hit.getObject();
    auto normal = object->getNormal(hit.getPosition());

    double diffuseTerm = 0.0;
    for (const auto& toLightDir : lightVisibilities)
    {
        if (!std::isnan(toLightDir.x))
            diffuseTerm = std::min(diffuseTerm + std::max(glm::dot(normal, toLightDir), 0.0), 1.0);
    }

    return object->getMaterial().getColor() * diffuseTerm;
}
