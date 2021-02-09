#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
   		this->center = center;
		this->forward = forward;
		this->up = up;
		this->scaleX = scaleX;
		this->scaleY = scaleY;

		this->CamZAxis = this->forward.normalize();
		this->CamXAxis = cross(this->CamZAxis, this->up.normalize()).normalize();
		this->CamYAxis = cross(this->CamXAxis, this->CamZAxis).normalize();
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
	Point o = this->center + (x * this->scaleX / 2.0f * this->CamXAxis + y * this->scaleY  / 2.0f * this->CamYAxis);
    return Ray(o, this->forward);
}

}
