#include <rt/solids/disc.h>
#include <core/random.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
    this->normal = normal;
    this->radius = radius;
    if(texMapper != nullptr) this->texMapper = texMapper;
    this->material = material;

		float ux = sqrt(this->normal.y * this->normal.y + this->normal.z * this->normal.z);
		float uy = sqrt(this->normal.x * this->normal.x + this->normal.z * this->normal.z);
		float uz = sqrt(this->normal.y * this->normal.y + this->normal.x * this->normal.x);

		Vector u = Vector(ux, uy, uz);
		Point bmin = this->center - this->radius * u;
		Point bmax = this->center + this->radius * u;

		bbox = BBox(bmin, bmax);
}

BBox Disc::getBounds() const {
    return bbox;
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {

    if (dot(ray.d, this->normal) == 0.0) return Intersection::failure();
    float t = - dot(ray.o - center, this->normal) / dot(ray.d, this->normal);

    if (t > previousBestDistance || t < 0) return Intersection::failure();
    Point hit_point = ray.getPoint(t);

    if ((hit_point - center).length() > radius) return Intersection::failure();
    return Intersection(t, ray, this, this->normal, hit_point);
}

Solid::Sample Disc::sample() const {
    float rx, ry, rz;
    float u = random();
    float v = sqrt(random());
    float r1 = v * radius * cos(2*pi*u);
    float r2 = v * radius * sin(2*pi*u);
    float r3 = v * radius * sin(2*pi*u);

    rx = r1 + center.x;
    ry = r2 + center.y;
    rz = r3 + center.z;

    return Sample(Point(rx, ry, rz), normal);
}

float Disc::getArea() const {
    return pi * pow(radius,2);
}

}
