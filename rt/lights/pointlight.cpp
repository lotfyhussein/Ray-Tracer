#include <rt/lights/pointlight.h>

#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
    this->position = position;
    this->intensity = intensity;
}

LightHit PointLight::getLightHit(const Point& p) const {
    Vector pointToLight = this->position - p;

    LightHit light_hit;
    light_hit.distance = pointToLight.length();
    light_hit.direction = pointToLight.normalize();
    light_hit.normal = -pointToLight;
    return light_hit;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return this->intensity / float(irr.distance * irr.distance);

}

}
