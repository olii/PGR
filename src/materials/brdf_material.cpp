#include "intersection.h"
#include "materials/brdf_material.h"
#include "shape.h"

BrdfMaterial::BrdfMaterial(const Color& color, double diffuseCoeff, double specularCoeff)
    : Material(color), _diffuseCoeff(diffuseCoeff), _specularCoeff(specularCoeff)
{
}

double BrdfMaterial::getDiffuseCoeff() const
{
    return _diffuseCoeff;
}

double BrdfMaterial::getSpecularCoeff() const
{
    return _specularCoeff;
}

Color BrdfMaterial::calculateColor(const Intersection& hit, const Scene&, const std::vector<const Light*>& visibleLights) const
{
    auto object = hit.getObject();
    auto normal = object->getNormal(hit.getPosition());

    double diffuseTerm = 0.0;
    for (const auto* light : visibleLights)
    {
        auto toLightDir = glm::normalize(light->getPosition() - hit.getPosition());
        diffuseTerm = std::min(diffuseTerm + std::max(glm::dot(normal, toLightDir), 0.0), 1.0);
    }

    return _color * diffuseTerm;
}
