#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
{
    this->origin = origin;
    this->normal = normal;
    if(texMapper != nullptr) this->texMapper = texMapper;
    this->material = material;
    bbox = BBox::full();
}

BBox InfinitePlane::getBounds() const {
    return bbox;
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
		if (dot(ray.d, this->normal) == 0.0) return Intersection::failure();
		float t = - dot(ray.o - origin, normal) / dot(ray.d, this->normal);
		if (t > previousBestDistance || t < 0) return Intersection::failure();
		return Intersection(t, ray, this, this->normal, ray.getPoint(t));
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return FLT_MAX;
}

}
