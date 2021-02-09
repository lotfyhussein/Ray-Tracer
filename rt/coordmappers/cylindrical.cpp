#include <rt/coordmappers/cylindrical.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
    this->origin = origin;
    this->longAx = longitudinalAxis;
    this->polAx = polarAxis;
    this->scaleY = longitudinalAxis.length();
    this->scaleX = polarAxis.length();
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    // Vector hitP = hit.local() - origin;

    // float v = dot(longAx.normalize(),hitP)/ longAx.length();

    // Vector perpPolar = (polAx - dot(longAx.normalize(),polAx)*longAx);
    // Vector z = cross(longAx.normalize(), polAx.normalize()).normalize() * polAx.length();
    // Vector polarHit = (hitP - v * longAx).normalize();

    // float u = acosf(dot(polarHit, z));
    // return Point(u,v,0);


	Vector hitP = hit.local() - origin;

    Vector y = cross(longAx.normalize(), polAx.normalize()).normalize() * polAx.length();

	float u = dot(hitP, polAx) / polAx.lensqr();
	float v = dot(hitP, y) / y.lensqr();

	float theta = atanf(v/u);
    float h = dot(longAx.normalize(),hitP)/ longAx.length();

	return Point(
        theta /(2*pi*scaleX), 
        h, 
        0.f);
}

}
