#include <rt/solids/triangle.h>
#include <core/random.h>

namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
{
		v1 = vertices[0];
		v2 = vertices[1];
		v3 = vertices[2];
		if(texMapper != nullptr) this->texMapper = texMapper;
		this->material = material;
}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		if(texMapper != nullptr) this->texMapper = texMapper;
		this->material = material;

		Vector v21 = (v2 - v1).normalize();
		Vector v31 = (v3 - v1).normalize();
		normal = cross(v21, v31).normalize();

		float minx = min(v1.x, v2.x, v3.x);
		float miny = min(v1.y, v2.y, v3.y);
		float minz = min(v1.z, v2.z, v3.z);

		float maxx = max(v1.x, v2.x, v3.x);
		float maxy = max(v1.y, v2.y, v3.y);
		float maxz = max(v1.z, v2.z, v3.z);

		Point min = Point(minx, miny, minz);
		Point max = Point(maxx, maxy, maxz);

		this->bbox = BBox(min, max);
}

BBox Triangle::getBounds() const {
    return this->bbox;
}

Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {

		if (dot(ray.d, this->normal) == 0.0) return Intersection::failure();
		float t = - dot(ray.o - v1, normal) / dot(ray.d, this->normal);

		if (t > previousBestDistance || t < 0.000001) return Intersection::failure();

		Point hit_point = ray.getPoint(t);

		Vector e1 = v2 - v1;
		Vector e2 = v3 - v1;
		Vector u = cross(e1, e2);

		Vector e11 = v2 - hit_point;
		Vector e12 = v3 - hit_point;
		Vector u1 = cross(e11, e12);

		Vector e21 = hit_point - v1;
		Vector e22 = v3 - v1;
		Vector u2 = cross(e21, e22);

		Vector e31 = v2 - v1;
		Vector e32 = hit_point - v1;
		Vector u3 = cross(e31, e32);

		float lambda1 =  u1.length() / u.length();
		float lambda2 = u2.length() / u.length();
		float lambda3 = u3.length() / u.length();

		float uu1 = dot(u, u1);
		float uu2 = dot(u, u2);
		float uu3 = dot(u, u3);

		if (uu1 < 0 || uu2 < 0 || uu3 < 0) return Intersection::failure();

		return Intersection(t, ray, this, normal, Point(lambda1, lambda2, lambda3));
}

Solid::Sample Triangle::sample() const {
		Vector e1 = v2 - v1;
		Vector e2 = v3 - v1;

		float r1 = random();
    float r2 = random();

    if((r1 + r2) >= 1.f){
        r1 = 1.f - r1;
        r2 = 1.f - r2;
    }
		return Sample((v1 + e1 * r1 + e2 * r2), normal);
}

float Triangle::getArea() const {
		return cross(v2 - v1, v3 - v1).length() / 2;
}

Point Triangle::getBarycentricCoords(const Point& p) const{
    Point barycentric;
		Vector e1 = v1 - p;
		Vector e2 = v2 - p;
		Vector e3 = v3 - p;
		float area = getArea();

    barycentric.x = (cross(e2, e3).length()/2) / area;
    barycentric.y = (cross(e1, e3).length()/2) / area;
    barycentric.z = (cross(e1, e2).length()/2) / area;
    assert(barycentric.x >=0  && barycentric.y >=0 && barycentric.z >=0);
    return barycentric;
}

}
