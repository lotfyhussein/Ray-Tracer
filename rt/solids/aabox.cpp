#include <rt/solids/aabox.h>
#include <core/point.h>
namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
{
		this->corner1 = corner1;
    this->corner2 = corner2;
    if(texMapper != nullptr) this->texMapper = texMapper;
    this->material = material;

    float minx  = std::min(corner1.x, corner2.x);
  	float miny  = std::min(corner1.y, corner2.y);
  	float minz  = std::min(corner1.z, corner2.z);

  	float maxx  = std::max(corner1.x, corner2.x);
  	float maxy  = std::max(corner1.y, corner2.y);
  	float maxz  = std::max(corner1.z, corner2.z);

  	Point min = Point(minx, miny, minz);
  	Point max = Point(maxx, maxy, maxz);

  	bbox = BBox(min, max);
}

BBox AABox::getBounds() const {
   return bbox;
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
		float x = std::abs(corner1.x - corner2.x);
		float y = std::abs(corner1.y - corner2.y);
		float z = std::abs(corner1.z - corner2.z);
		return 2 * (x*y + y*z + z*x) ;
}

Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    // We compute t_near (t0) and t_far (t1) for intersection with the aabox
    // ray.o.x + t0x * ray.d.x = corner1.x
    float t0x = (corner1.x - ray.o.x) / ray.d.x;
		float t1x = (corner2.x - ray.o.x) / ray.d.x;

		float t0y = (corner1.y - ray.o.y) / ray.d.y;
		float t1y = (corner2.y - ray.o.y) / ray.d.y;

		float t0z = (corner1.z - ray.o.z) / ray.d.z;
		float t1z = (corner2.z - ray.o.z) / ray.d.z;

    if (t0x > t1x) std::swap(t0x, t1x);
		if (t0y > t1y) std::swap(t0y, t1y);
		if (t0z > t1z) std::swap(t0z, t1z);

    // Now we get our intersection points.
		float t0 = max(max(t0x, t0y), t0z);
		float t1 = min(min(t1x, t1y), t1z);
		if (t0 >= t1)
        return Intersection::failure();

		bool is_t0_valid = t0 >= 0 && t0 <= previousBestDistance;
		bool is_t1_valid = t1 >= 0 && t1 <= previousBestDistance;

		if (!is_t0_valid && !is_t1_valid)
        return Intersection::failure();

		float t;
		if (is_t0_valid && is_t1_valid) t = min(t0, t1);
		if (!is_t0_valid && is_t1_valid) t = t1;
		if (is_t0_valid && !is_t1_valid) t = t0;

    // Prepare our normal vector
    Vector normal;
		if (t == t0) {
  			if (t == t0x){
  					normal = Vector(-1.f, 0, 0);
  			}
  			if (t == t0y) {
  					normal = Vector(0, -1.f, 0);
  			}
  			if (t == t0z) {
  					normal = Vector(0, 0, 1.f);
  			}
  			return Intersection(t, ray, this, normal, ray.getPoint(t));
		}


		if (t == t1) {
  			if (t == t1x) {
  					normal = Vector(1.f, 0, 0);
  			}
  			if (t == t1y) {
  					normal = Vector(0, 1.f, 0);
  			}
  			if (t == t1z) {
  					normal = Vector(0, 0, -1.f);
  			}
  			return Intersection(t, ray, this, normal, ray.getPoint(t));
		}
    return Intersection::failure();
}

}
