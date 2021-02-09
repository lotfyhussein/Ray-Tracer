#ifndef CG1RAYTRACER_RAY_HEADER
#define CG1RAYTRACER_RAY_HEADER

#include <core/vector.h>
#include <core/point.h>

namespace rt {

class Ray {
public:
    Point o;
    Vector d;
    float t; // time

    Ray() {}
    Ray(const Point& o, const Vector& d, float t = 0.f);
    Point getPoint(float distance) const;
};

}

#endif
