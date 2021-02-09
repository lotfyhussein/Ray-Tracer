#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const Float4& f4)
{
    this->x = f4.x / f4.w;
    this->y = f4.y / f4.w;
    this->z = f4.z / f4.w;
}

Vector Point::operator - (const Point& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

bool Point::operator == (const Point& b) const {
    return (this->x == b.x && this->y == b.y && this->z == b.z);
}

bool Point::operator != (const Point& b) const {
    return (this->x != b.x || this->y != b.y || this->z || b.z);
}

Point operator * (float scalar, const Point& b) {
    return Point(b.x * scalar, b.y * scalar, b.z * scalar);
}

Point operator * (const Point& a, float scalar) {
    return Point(a.x * scalar, a.y * scalar, a.z * scalar);
}

Point operator + (const Point& a, const Point& b){
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point min(const Point& a, const Point& b) {
    return Point(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Point max(const Point& a, const Point& b) {
    return Point(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

float Point::getAxis(const int index) const {
    if (index == 0)
        return x;
    else if (index == 1)
        return y;
    else
        return z;
}

}
