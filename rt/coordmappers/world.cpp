#include <rt/coordmappers/world.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    Float4 p = Float4(scale) * Float4(hit.local());
    return Point(p[0], p[1], p[2]);
}

WorldMapper::WorldMapper()
{
    this->scale = Vector::rep(1.0f);
}

WorldMapper::WorldMapper(const Vector& scale)
{
    this->scale = scale;
}

}
