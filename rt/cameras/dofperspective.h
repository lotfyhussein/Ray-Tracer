#ifndef CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER

#include <rt/cameras/camera.h>

namespace rt {

class Point;
class Vector;

class DOFPerspectiveCamera : public Camera {
public:
    DOFPerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle,
        float focalDistance,
        float apertureRadius
    );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    Point center;
    Vector forward;
    Vector up;
    float verticalOpeningAngle;
    float horizontalOpeningAngle;
    float scaleX;
    float scaleY;

    Vector CamXAxis;
    Vector CamYAxis;
    Vector CamZAxis;
    float fd, ar;
};

}

#endif