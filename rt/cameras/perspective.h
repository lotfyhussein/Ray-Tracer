#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle,
        float time0 = 0.f,
        float time1 = 0.f
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
        float time0;
        float time1;
};

}


#endif
