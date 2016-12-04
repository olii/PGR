#include <chrono>
#include <iostream>

#include <glm/gtx/norm.hpp>

#include "intersection.h"
#include "materials/bssrdf_material.h"
#include "ray.h"
#include "scene.h"
#include "shape.h"

BssrdfMaterial::BssrdfMaterial(const Color& color, const Color& absorbCoeff, const Color& scatterCoeff, double phase, double eta)
  : Material(color), _absorbCoeff(absorbCoeff), _scatterCoeff(scatterCoeff), _phase(phase), _eta(eta),
    _prng(std::chrono::system_clock::now().time_since_epoch().count())
{
    _reducedScatteringCoeff = _scatterCoeff * (1.0 - _phase);                                           // sigma_a
    _reducedExtinctionCoeff = _reducedScatteringCoeff + _absorbCoeff;                                   // sigma_t'
    _reducedAlbedo = _reducedScatteringCoeff / _reducedExtinctionCoeff;                                 // alpha'
    _effectiveTransportCoeff = colorLuminance(glm::sqrt(3.0 * _absorbCoeff * _reducedExtinctionCoeff)); // sigma_tr
    _fresnelDiffuseReflectance = _FdrIntegralApprox(_eta);                                              // F_dr
    _boundary = (1.0 + _fresnelDiffuseReflectance) / (1.0 - _fresnelDiffuseReflectance);                // A
    _positiveDipoleDistance = Color{1.0f, 1.0f, 1.0f} / _reducedExtinctionCoeff;                        // z_r
    //_negativeDipoleDistance = _positiveDipoleDistance + 4.0 * _boundary * (1.0 / (3.0 * _reducedExtinctionCoeff)); // z_v
    _negativeDipoleDistance = _positiveDipoleDistance * (1.0 + 4.0 / 3.0 * _boundary);   // z_v
}

const Color& BssrdfMaterial::getAbsorbCoeff() const
{
    return _absorbCoeff;
}

const Color& BssrdfMaterial::getScatterCoeff() const
{
    return _scatterCoeff;
}

Color BssrdfMaterial::calculateColor(const Intersection& hit, const Scene& scene, const std::vector<const Light*>&) const
{
    return _single(hit, scene) + _diffuse(hit, scene);
}

Color BssrdfMaterial::_single(const Intersection& hit, const Scene& scene) const
{
    return {};
}

Color BssrdfMaterial::_diffuse(const Intersection& hit, const Scene& scene) const
{
    // Fresnel out of surface (to camera)
    auto normalOut = hit.getObject()->getNormal(hit.getPosition());
    auto cameraDirOut = glm::normalize(scene.getCamera().getPosition() - hit.getPosition());
    double fresnelOut = _Fresnel(std::abs(glm::dot(cameraDirOut, normalOut)));

    //std::cout << "Fo = " << _Fresnel(cosOut) << std::endl;

    Color Sd{0.0};
    auto samples = _samplePoints(hit, scene);
    for (const auto& sample : samples)
    {
        // Rd function
        auto distance2 = glm::length2(hit.getPosition() - sample);
        Color Rd = _Rd(distance2);
        //std::cout << "RD = " << _Rd(distance2) << std::endl;

        // Fresnel to the surface (from light)
        auto normalIn = hit.getObject()->getNormal(sample);

        auto lights = scene.castShadowRays(sample, hit.getObject());
        if (lights.empty())
            continue;


        Color acc{0.0};
        for (const auto& light : lights)
        {
            auto lightDirIn = glm::normalize(light->getPosition() - sample);
            double cosIn = std::max( 0.0, glm::dot(lightDirIn, normalIn));
            double fresnelIn = _Fresnel(cosIn);
            acc += (1.0 / M_PI) * fresnelIn * Rd * fresnelOut * (light->getColor() * cosIn);
        }

        Sd += acc / static_cast<double>(lights.size());
    }

    Sd /= samples.size();
    return Sd;
}

std::vector<Vector> BssrdfMaterial::_samplePoints(const Intersection& hit, const Scene& scene) const
{
    std::vector<Vector> result;

    auto object = hit.getObject();
    auto position = hit.getPosition();
    auto normal = object->getNormal(position);

    //std::cout << "Hit point is " << position.x << " " << position.y << " " << position.z << std::endl;
    //std::cout << "Its normal is " << normal.x << " " << normal.y << " " << normal.z << std::endl;

    // Middle point of normal distribution
    auto middle = position + 0.001 * normal;

    // Find orthonormal basis based on normal vector
    auto right = glm::normalize(glm::cross(Vector{0.0, 1.0, 0.0}, normal));
    auto angle = std::abs(glm::dot(normal, Vector{0.0, 1.0, 0.0}));
    if (std::abs(angle - 1.0) < 1e-6)
        right = Vector{1.0, 0.0, 0.0};
    auto up = glm::normalize(glm::cross(normal, right));

    //std::cout << "ONB is [" << right.x << ";" << right.y << ";" << right.z << " "
    //    << up.x << ";" << up.y << ";" << up.z << " "
    //    << normal.x << ";" << normal.y << ";" << normal.z << std::endl;

    double Rmax = 0.1;
    double Rmax2 = Rmax * Rmax;

    for (std::uint32_t i = 0; i < 25; ++i)
    {
        // Just generate random number uniformly and calculate radius and angle
        double eps1 = static_cast<double>(_prng() % 1000) / 1000.0;
        double eps2 = static_cast<double>(_prng() % 1000) / 1000.0;
        double r =
            std::sqrt(std::log(1.0 - eps1 * (1.0 - std::exp(-_effectiveTransportCoeff * Rmax2))) / -_effectiveTransportCoeff);
        double theta = 2.0 * M_PI * eps2;
        //std::cout << "---------------- SAMPLE " << i << std::endl;

        // Transform back to cartesian coordinates
        double x = r * std::cos(theta);
        double y = r * std::sin(theta);

        //std::cout << "eps = " << eps << std::endl;
        //std::cout << "r = " << r << " and theta = " << theta << std::endl;
        //std::cout << "x = " << x << " and y = " << y << std::endl;

        // Find the origin of the probe ray in the local space
        auto origin = middle + x * right + y * up;
        //std::cout << "Probe ray origin has position " << origin.x << " " << origin.y << " " << origin.z << std::endl;

        // Cast probe ray down to the object in opposite direction of normal
        //Ray ray(origin, -normal);
        Ray ray(origin, object->getPosition() - origin);
        auto hit = object->intersects(ray);
        if (hit)
        {
            auto samplePos = hit.getPosition();
            result.push_back(samplePos);
            //std::cout << "Sampling point " << samplePos.x << " " << samplePos.y << " " << samplePos.z << std::endl;
        }
        //std::cout << std::endl;
    }

    return result;
}

double BssrdfMaterial::_FdrIntegralApprox(double eta)
{
    // Approximation of fresnel integral
    if (eta < 1.0f)
        return -0.4399 + 0.7099 / eta - 0.3319 / (eta * eta) + 0.0636 / (eta * eta * eta);
    else
        return -1.4399 / (eta * eta) + 0.7099 / eta + 0.6681 + 0.0636 * eta;
}

Color BssrdfMaterial::_Rd(double distance2) const
{
    // Rd function from Jensen
    Color pDipoleSampleDist = glm::sqrt(_positiveDipoleDistance * _positiveDipoleDistance + Color{distance2});
    Color nDipoleSampleDist = glm::sqrt(_negativeDipoleDistance * _negativeDipoleDistance + Color{distance2});

    Color pdsd3 = pDipoleSampleDist * pDipoleSampleDist * pDipoleSampleDist;
    Color ndsd3 = nDipoleSampleDist * nDipoleSampleDist * nDipoleSampleDist;

    return (_reducedAlbedo / (4.0 * M_PI)) *
        ((_effectiveTransportCoeff * pDipoleSampleDist + 1.0) * glm::exp(-_effectiveTransportCoeff * pDipoleSampleDist) /
                   (_reducedExtinctionCoeff * pdsd3) +
               _negativeDipoleDistance * (_effectiveTransportCoeff * nDipoleSampleDist + 1.0) *
                   glm::exp(-_effectiveTransportCoeff * nDipoleSampleDist) / (_reducedExtinctionCoeff * ndsd3));
}

double BssrdfMaterial::_Fresnel(double angle) const
{
    double etai = 1.0;
    double sint = (etai / _eta) * std::sqrt(std::max(0.0, 1.0 - angle * angle));
    if (sint >= 1.0)
    {
        return 0.0;
    }
    double cost = sqrt(std::max(0.0, 1.0 - sint * sint));
    double cosi = std::abs(angle);

    float rParl = ((_eta * cosi) - (etai * cost)) / ((_eta * cosi) + (etai * cost));
    float rPerp = ((etai * cosi) - (_eta * cost)) / ((etai * cosi) + (_eta * cost));
    return 1.0 - (rParl * rParl + rPerp * rPerp) / 2.0f;
}
