#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{
    this->origin = origin;
    this->zen = zenith;
    this->aR = azimuthRef;
    this->scaleX = aR.length();
    this->scaleY = zen.length();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    // Vector hitP = hit.local() - origin;
    // float v = acos(dot(z.normalize(),hitP.normalize()));

    // Vector perpPolar = (aR - dot(z.normalize(),aR)*z);
    // Vector polarHit = (hitP -  dot(z.normalize(),hitP)* z).normalize();

    // float u = acos(dot(polarHit, perpPolar));
    // return Point(u,v,0);



    Vector hitP = hit.local() - origin;

    Vector y = cross(zen.normalize(), aR.normalize()).normalize() * aR.length();

    // Get x and y cord of the projection of local hit on x and y direction. 
    float x_cord = dot(hitP, aR) / aR.lensqr();
    float y_cord = dot(hitP, y) / y.lensqr();
    // phi is the angle between this vector and the x direction
    float phi = atanf(y_cord/x_cord);
    float theta = acosf(dot(hitP.normalize(), zen.normalize()));
    //Normalize
    phi = phi / (2 * pi * aR.length());
    theta = theta / (pi * aR.length());

    return Point(phi, theta, 0.0f);
}

}
