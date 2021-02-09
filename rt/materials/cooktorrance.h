#ifndef CG1RAYTRACER_MATERIALS_COOKTORRANCE_HEADER
#define CG1RAYTRACER_MATERIALS_COOKTORRANCE_HEADER

#include <rt/materials/material.h>

namespace rt {
class Texture;

class CookTorrance : public Material {
public:
    CookTorrance(Texture *glossy, Texture *diffuse, float kappa, float eta, float m);
    virtual RGBColor getReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir, const Vector &inDir) const;
    virtual RGBColor getEmission(const Point &texPoint, const Vector &normal, const Vector &outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir) const;
    virtual Sampling useSampling() const;

    RGBColor getFresnel(const Vector &normal, const Vector &inDir, const Vector &outDir) const;
    float getGeometricAttenuation(const Vector &normal, const Vector &inDir, const Vector &outDir) const;
    float getMicrofacetDistribution(const Vector &normal, const Vector &inDir, const Vector &outDir) const;

private:
    Texture *glossy, *diffuse;
    float kappa, eta, m;
};

}

#endif
