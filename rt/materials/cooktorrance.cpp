#include <rt/materials/cooktorrance.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>
#include <cmath>
#include <algorithm>

namespace rt {

CookTorrance::CookTorrance(rt::Texture *glossy, rt::Texture *diffuse, float kappa, float eta, float m)
{
    this->glossy = glossy;
    this->diffuse = diffuse;
    this->kappa = kappa;
    this->eta = eta;
    this->m = (m) ? m : 0.02f;
}

RGBColor CookTorrance::getReflectance(const rt::Point &texPoint, const rt::Vector &normal, const rt::Vector &outDir,const rt::Vector &inDir) const {
    return (1.f/pi) * dot(normal, -inDir) * diffuse->getColor(texPoint);
}

RGBColor CookTorrance::getEmission(const rt::Point &texPoint, const rt::Vector &normal,const rt::Vector &outDir) const {
    return RGBColor::rep(0.f);
}

Material::SampleReflectance CookTorrance::getSampleReflectance(const rt::Point &texPoint, const rt::Vector &normal,const rt::Vector &outDir) const {
    // return mirror sample reflectance.
    Vector refDir = (-outDir + 2 * dot(outDir, normal) * normal).normalize();

    RGBColor F = getFresnel(normal, refDir, outDir);
    float D = getMicrofacetDistribution(normal, refDir, outDir);
    float G = getGeometricAttenuation(normal, refDir, outDir);

    RGBColor kd = (1.f/pi) * dot(normal, refDir) * diffuse->getColor(texPoint);
    RGBColor kg = F * D * G / (pi * dot(normal, outDir) * dot(normal, refDir));

    // pd + pg <= 1, we assume 0.5 for each
    float pd = 0.5f, pg = 0.5f;
    RGBColor fr = kd*pd + kg*pg;

    return SampleReflectance(refDir, fr);
}

Material::Sampling CookTorrance::useSampling() const {
    return SAMPLING_SECONDARY;
}

RGBColor CookTorrance::getFresnel(const Vector &normal, const Vector &inDir, const Vector &outDir) const {
    Vector refDir = (-outDir + 2 * dot(outDir, normal) * normal).normalize();

    float cosThetaIn = dot(inDir, normal);
    float cosThetaOut = dot(refDir, normal);
    float delta = fabs(cosThetaIn - cosThetaOut) == 0.f ? 1.f : 0.f;

    float r1 = (kappa*cosThetaIn - eta*cosThetaOut) / (kappa*cosThetaIn + eta*cosThetaOut);
    float r2 = (eta*cosThetaIn - kappa*cosThetaOut) / (kappa*cosThetaIn + eta*cosThetaOut);
    float Fr = 0.5f * (r1 + r2);
    float brdf = Fr * delta/fabs(cosThetaIn);
    return RGBColor::rep(brdf);
}

float CookTorrance::getGeometricAttenuation(const rt::Vector &normal, const rt::Vector &inDir,const rt::Vector &outDir) const {
    Vector H = (inDir + outDir).normalize();
    float pmor = 2.f * dot(normal, H) * dot(normal, outDir) / dot(outDir, H);
    float psoi = 2.f * dot(normal, H) * dot(normal, inDir) / dot(outDir, H);
    return std::min(1.f, min(pmor, psoi));
    // Ref: https://computergraphics.stackexchange.com/questions/4394/path-tracing-the-cook-torrance-brdf
}

float CookTorrance::getMicrofacetDistribution(const rt::Vector &normal, const rt::Vector &inDir,const rt::Vector &outDir) const {
    Vector H = (inDir + outDir).normalize();
    float alpha = dot(normal.normalize(), H);
    float exponent = powf(tan(alpha)/m, 2);
    return (1.f/(pi * m * m * powf(cos(alpha), 4))) * exp(-exponent);
}

}
