#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
	this->ray = ray;
	this->solid = solid;
    this->normalV = normal;
    this->localP = local;
    this->IntersectionOccurred = true;
}

Intersection::operator bool() {
    return (this->IntersectionOccurred);
}

Intersection Intersection::failure() {
	Intersection FailedIntersection;
	FailedIntersection.IntersectionOccurred = false;
	return FailedIntersection;
}

Point Intersection::hitPoint() const {
    return (this->ray.getPoint(this->distance));
}

Vector Intersection::normal() const {
    return (this->normalV);
}

Point Intersection::local() const {
    return (this->localP);
}

}
