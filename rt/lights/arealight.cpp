#include <rt/lights/arealight.h>
#include <rt/materials/material.h>
#include <rt/solids/solid.h>

#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
    LightHit lightHit;
    Solid::Sample sample = source->sample();

    lightHit.direction = (sample.point - p).normalize();
    lightHit.normal = sample.normal;
    lightHit.distance = (sample.point - p).length() - offset;

    return lightHit;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    RGBColor emission = source->material->getEmission(Point::rep(0.f), Vector::rep(0.f), Vector::rep(0.f));
    RGBColor power = emission * source->getArea();
    return power  * (dot(irr.normal, -irr.direction) / (irr.distance * irr.distance));
}

AreaLight::AreaLight(Solid* source)
{
    this->source = source;
}

}
