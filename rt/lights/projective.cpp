#include <rt/lights/projective.h>

namespace rt {

ProjectiveLight::ProjectiveLight(const Point& position, const RGBColor& intensity) {
		this->pos = position;
		this->intensity = intensity;
}

LightHit ProjectiveLight::getLightHit(const Point& p) const{
		Vector pointToLight = this->pos - p;

		LightHit light_hit;
		light_hit.distance = pointToLight.length();
		light_hit.direction = pointToLight.normalize();
		light_hit.normal = -pointToLight;
		return light_hit;
}

float a2computeWeight(float fx, float fy, const Point& c, float div) {
    Point v(fx, fy, 0.0f);
    int numIter = julia(v, c);
    return numIter/(numIter+div);
}

RGBColor ProjectiveLight::getIntensity(const LightHit& irr) const {
		float theta = asin(irr.direction.z)/pi*2;
		float phi = atan2(irr.direction.y,irr.direction.x)/pi;
		float ofx = absfractional((pos.x+1.0f)/2.0f)*2.0f-1.0f;
		float ofy = absfractional((pos.y+1.0f)/2.0f)*2.0f-1.0f;
		RGBColor color = RGBColor::rep(0.0f);
		color = color + a2computeWeight(phi, theta, Point(-0.8f, 0.156f, 0.0f), 64.0f) * RGBColor(0.8f, 0.8f, 1.0f);
		color = color + a2computeWeight(phi, theta, Point(-0.6f, 0.2f, 0.0f), 64.0f)*0.2f * RGBColor(0.5f, 0.5f, -0.2f);
		color = color + a2computeWeight(ofy, ofx, Point(0.285f, 0.0f, 0.0f), 64.0f) * RGBColor(0.4f, 0.5f, 0.6f);
		color = RGBColor::rep(1.0f) - color;
		if (absfractional(theta/(2*pi)*90)<0.03) color = RGBColor(0.9f,0.5f,0.5f)*0.7f;
		if (absfractional(phi/(2*pi)*90)<0.03) color = RGBColor(0.9f,0.9f,0.5f)*0.7f;
		return color;
	}
}
