#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <core/random.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
{
    this->v1 = origin;
		this->span1 = span1;
		this->span2 = span2;
		this->normal = cross(span1, span2).normalize();
    if(texMapper != nullptr) this->texMapper = texMapper;
    this->material = material;

		this->v2 = v1 + this->span1;
		this->v3 = v1 + this->span1 + this->span2;
		this->v4 = v1 + this->span2;

		this->center = v1 + (0.5*this->span1) + (0.5*this->span2);

		float minx = min(v1.x, v2.x, min(v3.x, v4.x));
		float miny = min(v1.y, v2.y, min(v3.y, v4.y));
		float minz = min(v1.z, v2.z, min(v3.z, v4.z));

		float maxx = max(v1.x, v2.x, max(v3.x, v4.x));
		float maxy = max(v1.y, v2.y, max(v3.y, v4.y));
		float maxz = max(v1.z, v2.z, max(v3.z, v4.z));

		bbox = BBox(Point(minx, miny, minz), Point(maxx, maxy, maxz));
}

BBox Quad::getBounds() const {
    return bbox;
}

Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    if (dot(ray.d, this->normal) == 0.0) return Intersection::failure();
    float t = dot(center - ray.o, normal) / dot(ray.d, this->normal);

    if (t > previousBestDistance || t < 0.001) return Intersection::failure();
    Point hit_point = ray.getPoint(t);

    bool check1 = dot(cross(v2-v1, hit_point - v1), normal) >= 0;
    bool check2 = dot(cross(v3-v2, hit_point - v2), normal) >= 0;
    bool check3 = dot(cross(v4-v3, hit_point - v3), normal) >= 0;
    bool check4 = dot(cross(v1-v4, hit_point - v4), normal) >= 0;

    if (check1 && check2 && check3 && check4)
        return Intersection(t, ray, this, normal, getBarycentricCoords(hit_point));
    else
        return Intersection::failure();
}

Solid::Sample Quad::sample() const {
	return {v1 + random() * span1 + random() * span2, normal};
}

float Quad::getArea() const {
	 return cross(span1, span2).length();
}

Point Quad::getBarycentricCoords(const Point& p) const{
    Vector op = p - v1;
    float u, v;
    if (span1.z * span2.y != 0.0f || span2.z * span1.y != 0.0f){
        u = (op.z * span2.y - op.y * span2.z)/(span1.z * span2.y - span1.y * span2.z);
        v = (op.z * span1.y - op.y * span1.z)/(span2.z * span1.y - span2.y * span1.z);
    }
    else if(span1.x * span2.y != 0.0f || span1.y * span2.x != 0.0f){
        u = (op.x * span2.y - op.y * span2.x)/(span1.x * span2.y - span1.y * span2.x);
        v = (op.x * span1.y - op.y * span1.x)/(span2.x * span1.y - span2.y * span1.x);
    }
    else if(span1.z * span2.x != 0.0f || span1.x * span2.z != 0.0f){
        u = (op.z * span2.x - op.x * span2.z)/(span1.z * span2.x - span1.x * span2.z);
        v = (op.z * span1.x - op.x * span1.z)/(span2.z * span1.x - span2.x * span1.z);
    }
    return Point(u, v, 0.0f);
}

}
