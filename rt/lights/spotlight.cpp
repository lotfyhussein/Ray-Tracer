#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
    this->pos = position;
    this->dir = direction.normalize();
    this->angle = angle;
    this->exp = power;
    this->intensity = intensity;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {

    float cos_theta  = dot(-irr.direction.normalize(), this->dir);

    if (cos_theta <= cos(angle) || cos_theta < 0)
        return RGBColor::rep(0.0f);

    return intensity * pow((cos_theta), exp) / float(irr.distance * irr.distance);

}

LightHit SpotLight::getLightHit(const Point& p) const {
    Vector pointToLight = this->pos - p;

    LightHit light_hit;
    light_hit.distance = pointToLight.length();
    light_hit.direction = pointToLight.normalize();
    light_hit.normal = -pointToLight;
    return light_hit;
	}
}
