#include <rt/primmod/bmap.h>
#include <core/assert.h>

namespace rt {

BumpMapper::BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale)
{
    this->base = base;
    this->bumpmap = bumpmap;
    this->bv1 = bv1;
    this->bv2 = bv2;
    this->bv3 = bv3;
    this->vscale = vscale;

}

BBox BumpMapper::getBounds() const {
    return base->getBounds();
}

// TODO: A full implementation has to make use of the coord mappers as well!
Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection Intersec = this->base->intersect(ray, previousBestDistance);
    if (!Intersec) return Intersection::failure();

    TriangleMapper* tmapper = new TriangleMapper(bv1, bv2, bv3);
    Point p = tmapper->getCoords(Intersec);

    //world space basis
    Vector wz = Intersec.normal().normalize();
    Vector e = Vector(1, 0, 0);
	if (abs(wz.x) >= 1) 
        e = Vector(0, 1, 0); // in case wz is (1, 0, 0) or (-1, 0, 0)
	Vector wy = cross(wz, e).normalize();
    Vector wx = cross(wy, wz).normalize();

    // Get DX and DY
    RGBColor dx = bumpmap->getColorDX(p);
    RGBColor dy = bumpmap->getColorDY(p);
    float DX = (dx.r + dx.g + dx.b) / 3.0f;
    float DY = (dy.r + dy.g + dy.b) / 3.0f;

    Vector n = (wx * DX + wy * DY + wz).normalize();

	return Intersection(Intersec.distance, Intersec.ray, Intersec.solid, n, Intersec.local());

}

void BumpMapper::setMaterial(Material* m) {
    base->setMaterial(m);
}

void BumpMapper::setCoordMapper(CoordMapper* cm) {
    base->setCoordMapper(cm);
}

float BumpMapper::getArea() const {
    return 0;
}

}