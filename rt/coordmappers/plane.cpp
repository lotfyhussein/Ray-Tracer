#include <rt/coordmappers/plane.h>
#include <core/matrix.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    this->e1 = e1;
    this->e2 = e2;
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    Vector normal = cross(e1, e2);
    Matrix m = Matrix::system(e1, e2, normal).invert();
    return m * hit.local();
}

}
