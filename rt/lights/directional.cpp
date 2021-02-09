#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
    this->dir = direction;
    this->color = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {

    LightHit light_hit;
    light_hit.direction = -this->dir;
    light_hit.normal = this->dir;
    light_hit.distance = FLT_MAX;
    return light_hit;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return this->color;
}

}
