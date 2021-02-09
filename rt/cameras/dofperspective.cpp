#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <core/random.h>
#include <math.h>
#include <core/scalar.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
{
    // Initialize the private varibales
   	this->center = center;
  	this->forward = forward;
  	this->up = up;
    this->verticalOpeningAngle = verticalOpeningAngle;
    this->horizontalOpeningAngle = horizontalOpeningAngle;

    // Calculate scaleX and scaleY from the horizontal and vertical angles
    this->scaleX = tan(this->horizontalOpeningAngle/2.0f);
    this->scaleY = tan(this->verticalOpeningAngle/2.0f);

    // Finally calibrate the camera's x, y, z axis.
    this->CamZAxis = this->forward.normalize();
    this->CamXAxis = cross(this->forward, this->up.normalize()).normalize();
    this->CamYAxis = cross(this->CamXAxis, this->CamZAxis).normalize();

    this->fd = focalDistance;
    this->ar = apertureRadius;
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {

    Point fp = center + (CamZAxis + x * CamXAxis * scaleX + y * CamYAxis * scaleY).normalize() * fd;
	float random_ar = random(0.0f, ar);
	float angle = 2 * pi * random();
	x = random_ar * cos(angle);
	y = random_ar * sin(angle);

	Point o = center + x * CamXAxis + y * CamYAxis;
	Vector d = (fp - o).normalize();

	return Ray(o, d);
}

}