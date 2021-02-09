#include <rt/bbox.h>
#include <rt/ray.h>

namespace rt {

BBox BBox::empty() {
    BBox bbox;
    bbox.isEmpty = true;
    return bbox;
}

BBox BBox::full() {
    Point minimum = Point(__FLT_MIN__, __FLT_MIN__, __FLT_MIN__);
    Point maximum = Point(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__);
    BBox bbox = BBox(minimum, maximum);
    bbox.isEmpty = false;
    return bbox;
}

void BBox::extend(const Point& point) {
    if(this->isEmpty) {
        this->isEmpty = false;
        min = point;
        max = point;
        return;
    }
    else {
        if (min.x > point.x) min.x = point.x;
    		if (min.y > point.y) min.y = point.y;
    		if (min.z > point.z) min.z = point.z;

    		if (max.x < point.x) max.x = point.x;
    		if (max.y < point.y) max.y = point.y;
    		if (max.z < point.z) max.z = point.z;
        return;
    }
}

void BBox::extend(const BBox& bbox) {
    if (this->isEmpty) {
        this->isEmpty = false;
        min = bbox.min;
        max = bbox.max;
        return;
    }
    else {
        if (min.x > bbox.min.x) min.x = bbox.min.x;
        if (min.y > bbox.min.y) min.y = bbox.min.y;
        if (min.z > bbox.min.z) min.z = bbox.min.z;

        if (max.x < bbox.max.x) max.x = bbox.max.x;
        if (max.y < bbox.max.y) max.y = bbox.max.y;
        if (max.z < bbox.max.z) max.z = bbox.max.z;
    }
    return;
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {
    bool isFull = this->isUnbound();
    if(isFull)
        return std::make_pair(__FLT_MIN__, __FLT_MAX__);
    else if(this->isEmpty)
        return std::make_pair(__FLT_MAX__, __FLT_MIN__);
    else {
        float t0x = (min.x - ray.o.x) / ray.d.x;
        float t1x = (max.x - ray.o.x) / ray.d.x;

        float t0y = (min.y - ray.o.y) / ray.d.y;
        float t1y = (max.y - ray.o.y) / ray.d.y;

        float t0z = (min.z - ray.o.z) / ray.d.z;
        float t1z = (max.z - ray.o.z) / ray.d.z;

        if (t0x > t1x) std::swap(t0x, t1x);
        if (t0y > t1y) std::swap(t0y, t1y);
        if (t0z > t1z) std::swap(t0z, t1z);

        // Now we get our intersection points.
        float t0 = std::max((t0x, t0y), t0z);
        float t1 = std::min((t1x, t1y), t1z);

        return std::make_pair (t0, t1);
    }
}

bool BBox::isUnbound() const {
		if (min.x == __FLT_MIN__ || min.y == __FLT_MIN__ || min.z == __FLT_MIN__ ||
            max.x == __FLT_MAX__ || max.y == __FLT_MAX__ || max.z == __FLT_MAX__)
        return true;
    else
        return false;
}

int BBox::largestAxisIndex() {
    Vector d = diagonal();
    if(d.x > d.y && d.x > d.z)
        return 0;
    else if(d.y > d.z)
        return 1;
    else
        return 2;
}

Point BBox::getCenter() const {
    Point center;
    center.x = (min.x + max.x)/2;
    center.y = (min.y + max.y)/2;
    center.z = (min.z + max.z)/2;
    return center;
}

}
