#include <chrono>
#include <thread>
#include <sstream>

#include <glm/gtx/norm.hpp>

#include "intersection.h"
#include "materials/bssrdf_material.h"
#include "multithreading/thread_safe_prng.h"
#include "ray.h"
#include "scene.h"
#include "settings.h"
#include "shape.h"

BssrdfMaterial::BssrdfMaterial(const Color& absorbCoeff, const Color& scatterCoeff, double phase, double eta)
  : Material(Color{}), _absorbCoeff(absorbCoeff), _scatterCoeff(scatterCoeff), _phase(phase), _eta(eta)
{
    _reducedScatteringCoeff = _scatterCoeff * (1.0 - _phase);                                             // sigma_a
    _reducedExtinctionCoeff = _reducedScatteringCoeff + _absorbCoeff;                                     // sigma_t'
    _reducedAlbedo = _reducedScatteringCoeff / _reducedExtinctionCoeff;                                   // alpha'
    _effectiveTransportCoeff = colorLuminance(glm::sqrt(3.0 * _absorbCoeff * _reducedExtinctionCoeff));   // sigma_tr
    _fresnelDiffuseReflectance = _FdrIntegralApprox(_eta);                                                // F_dr
    _boundary = (1.0 + _fresnelDiffuseReflectance) / (1.0 - _fresnelDiffuseReflectance);                  // A
    _positiveDipoleDistance = Color{1.0f, 1.0f, 1.0f} / _reducedExtinctionCoeff;                          // z_r
    _negativeDipoleDistance = _positiveDipoleDistance * (1.0 + 4.0 / 3.0 * _boundary);                    // z_v
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

inline double exponentialPdf(double x, double falloff)
{
    return falloff * std::exp(-falloff * x);
}

Color BssrdfMaterial::_single(const Intersection& hit, const Scene& scene) const
{
    auto object = hit.getObject();

    auto rayOut = -hit.getRayDirection();
    auto normalOut = object->getNormal(hit.getPosition());
    auto refractOut = _refract(rayOut, normalOut);
    double cosOut = std::max(0.0, glm::dot(normalOut, rayOut));

    double fresnelOut = _Fresnel(cosOut);
    double falloff = colorLuminance(_reducedExtinctionCoeff);

    Color S1{0.0};

    std::size_t numSamples = Settings::instance().getSingleScatterSamplesCount();
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        double distance = getRandomExponential(falloff);
        auto volumePosition = hit.getPosition() + distance * refractOut;

        Color acc{0.0};
        for (const auto& light : scene.getLights())
        {
            Ray ray(volumePosition, light->getPosition() - volumePosition);
            auto selfHit = object->intersects(ray);
            if (!selfHit)
                continue;

            auto positionIn = selfHit.getPosition();
            auto normalIn = object->getNormal(positionIn);
            if (!scene.castShadowRay(positionIn, object, light.get()))
                continue;

            auto rayIn = glm::normalize(light->getPosition() - positionIn);
            double cosIn = std::max(0.0, glm::dot(normalIn, rayIn));
            double fresnelIn = _Fresnel(cosIn);
            double geometryFactor = std::max(0.0, glm::dot(normalIn, refractOut)) / cosIn;
            Color combinedExtinctionCoeff = _reducedExtinctionCoeff + geometryFactor * _reducedExtinctionCoeff;
            double invEta2 = 1.0 / _eta;
            invEta2 *= invEta2;
            double volumeSampleDistanceToIn = glm::length(volumePosition - positionIn);
            double photonTravelDistance = volumeSampleDistanceToIn * cosIn / (std::sqrt(1.0 - invEta2 * (1.0 - cosIn * cosIn)));
            double p = _phaseFunction(rayIn, refractOut);

            acc += ((_scatterCoeff * fresnelIn * fresnelOut * p) / combinedExtinctionCoeff) *
                glm::exp(-photonTravelDistance * _reducedExtinctionCoeff) * glm::exp(-distance * _reducedExtinctionCoeff) *
                light->getColor();
        }
        acc /= scene.getLights().size();
        S1 += acc;
    }

    S1 /= numSamples;
    return S1;
}

Color BssrdfMaterial::_diffuse(const Intersection& hit, const Scene& scene) const
{
    // Fresnel out of surface (to camera)
    auto normalOut = hit.getObject()->getNormal(hit.getPosition());
    auto cameraDirOut = glm::normalize(scene.getCamera().getPosition() - hit.getPosition());
    double fresnelOut = _Fresnel(std::max(0.0, glm::dot(cameraDirOut, normalOut)));

    Color Sd{0.0};
    auto samples = _samplePoints(hit, scene);
    for (const auto& samplePair : samples)
    {
        const auto& sample = samplePair.first;

        // Rd function
        auto distance2 = glm::length2(hit.getPosition() - sample);
        Color Rd = _Rd(distance2);

        // Fresnel to the surface (from light)
        auto normalIn = hit.getObject()->getNormal(sample);

        auto lights = scene.castShadowRays(sample, hit.getObject());
        if (lights.empty())
            continue;

        Color acc{0.0};
        for (const auto& light : lights)
        {
            auto lightDirIn = glm::normalize(light->getPosition() - sample);
            double cosIn = std::max(0.0, glm::dot(lightDirIn, normalIn));
            double fresnelIn = _Fresnel(cosIn);
            acc += (1.0 / M_PI) * fresnelIn * Rd * fresnelOut * (light->getColor() * cosIn);
        }

        acc /= samplePair.second;
        acc /= static_cast<double>(lights.size());

        Sd += acc;
    }

    Sd /= samples.size();
    return Sd;
}

double gaussianSample2DPdf(float dist, float falloff)
{
    return (1.0 / M_PI) * falloff * exp(-falloff * (dist * dist));
}

inline double gaussianSample2DPdf(double dist, double falloff, double Rmax2)

{
    return gaussianSample2DPdf(dist, falloff) / (1.0 - exp(-falloff * Rmax2));
}

std::vector<std::pair<Vector, double>> BssrdfMaterial::_samplePoints(const Intersection& hit, const Scene&) const
{
    std::vector<std::pair<Vector, double>> result;

    auto object = hit.getObject();
    auto position = hit.getPosition();
    auto normal = object->getNormal(position);

    // Middle point of normal distribution
    auto middle = position + 0.001 * normal;

    // Find orthonormal basis based on normal vector
    auto right = glm::normalize(glm::cross(Vector{0.0, 1.0, 0.0}, normal));
    auto angle = std::abs(glm::dot(normal, Vector{0.0, 1.0, 0.0}));
    if (std::abs(angle - 1.0) < 1e-6)
        right = Vector{1.0, 0.0, 0.0};
    auto up = glm::normalize(glm::cross(normal, right));

    double Ratio = 6.0;
    double Rmax = std::sqrt(1.0 / (2.0 * _effectiveTransportCoeff * Ratio));
    double Rmax2 = Rmax * Rmax;

    std::size_t numSamples = Settings::instance().getMultiScatterSamplesCount();
    for (std::uint32_t i = 0; i < numSamples; ++i)
    {
        // Just generate random number uniformly and calculate radius and angle
        double eps1 = getRandomUniform();
        double eps2 = getRandomUniform();
        double r =
            std::sqrt(std::log(1.0 - eps1 * (1.0 - std::exp(-_effectiveTransportCoeff * Rmax2))) / -_effectiveTransportCoeff);
        double theta = 2.0 * M_PI * eps2;

        // Transform back to cartesian coordinates
        double x = r * std::cos(theta);
        double y = r * std::sin(theta);

        // Find the origin of the probe ray in the local space
        auto origin = middle + x * right + y * up;

        // Cast probe ray down to the object in opposite direction of normal
        //Ray ray(origin, -normal);
        Ray ray(origin, object->getPosition() - origin);
        auto hit = object->intersects(ray);
        if (hit)
        {
            auto samplePos = hit.getPosition();
            auto pdf = gaussianSample2DPdf(r, _effectiveTransportCoeff, Rmax2);
            result.push_back(std::make_pair(samplePos, pdf));
        }
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

    double sPolar = ((_eta * cosi) - (etai * cost)) / ((_eta * cosi) + (etai * cost));
    double pPolar = ((etai * cosi) - (_eta * cost)) / ((etai * cosi) + (_eta * cost));
    return 1.0 - (sPolar * sPolar + pPolar * pPolar) / 2.0;
}

Vector BssrdfMaterial::_refract(const Vector& vec, const Vector& normal) const
{
    double cosine = std::max(0.0, glm::dot(vec, normal));
    double eta = 1.0 / _eta;
    return glm::normalize(normal * (eta * cosine - std::sqrt(1.0 - eta * eta * (1.0 - cosine * cosine))) - eta * vec);
}

double BssrdfMaterial::_exponentialDistribution(double x, double lambda) const
{
    return -std::log(x) / lambda;
}

double BssrdfMaterial::_exponentialPdf(double x, double lambda) const
{
    return lambda * std::exp(-lambda * x);
}

double BssrdfMaterial::_phaseFunction(const Vector& v1, const Vector& v2) const
{
    double cosine = glm::dot(v1, v2);
    double phase2 = _phase * _phase;
    double denom = 1.0 + phase2 - 2.0 * _phase * cosine;
    double denom3 = denom * denom * denom;
    return (0.25 / M_PI) * ((1.0 - phase2) / std::sqrt(denom3));
}
