#include <rt/materials/mirror.h>
#include <core/scalar.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa)
{
    this->eta = eta;
    this->kappa = kappa;
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0.f);
}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.f);
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    // electric conductors from the class
    Vector refDir = (-outDir + 2 * dot(outDir, normal) * normal).normalize();

    float squares = (eta*eta) + (kappa*kappa);
    float cosTheta = dot(refDir, normal);
    float cosThetaSq = cosTheta * cosTheta;

    float rPar = ((squares * cosThetaSq) - (2 * eta * cosTheta) + 1) / ((squares * cosThetaSq) + (2 * eta * cosTheta) + 1);
    float rPer = (squares - (2 * eta * cosTheta) + cosThetaSq) / (squares + (2 * eta * cosTheta) + cosThetaSq);

    return SampleReflectance(refDir, RGBColor::rep(0.5f * (rPar + rPer)));
}

Material::Sampling MirrorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}
