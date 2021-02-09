#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const Float4& f4)
{
    this->x = f4.x - f4.w;
    this->y = f4.y - f4.w;
    this->z = f4.z - f4.w;
}

Vector Vector::operator + (const Vector& b) const {
    return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector Vector::operator - (const Vector& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

Vector Vector::operator - () const {
    return Vector(this->x * -1, this->y * -1, this->z *-1);
}

Vector Vector::normalize() const {
    return Vector(this->x/this->length(), this->y/this->length(), this->z/this->length());
}

Vector operator * (float scalar, const Vector& b) {
    return Vector(b.x * scalar, b.y * scalar, b.z * scalar);
}

Vector operator * (const Vector& a, float scalar) {
    return Vector(a.x * scalar, a.y * scalar, a.z * scalar);
}

Vector operator / (const Vector& a, float scalar) {
    return Vector(a.x / scalar, a.y / scalar, a.z / scalar);
}

Vector cross(const Vector& a, const Vector& b) {

  float r_x = a.y*b.z - a.z*b.y;
  float r_y = a.z*b.x - a.x*b.z;
  float r_z = a.x*b.y - a.y*b.x;
  return Vector(r_x, r_y, r_z);
}

float dot(const Vector& a, const Vector& b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float Vector::lensqr() const {
    return pow(this->length(), 2);
}

float Vector::length() const {
    return (sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)));
}


bool Vector::operator == (const Vector& b) const {
    return (this->x == b.x && this->y == b.y && this->z == b.z);
}

bool Vector::operator != (const Vector& b) const {
    return (this->x != b.x || this->y != b.y || this->z || b.z);
}

Vector min(const Vector& a, const Vector& b) {
    return Vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Vector max(const Vector& a, const Vector& b) {
    return Vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

Point operator + (const Point& a, const Vector& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator + (const Vector& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator - (const Point& a, const Vector& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator * (const Float4& scale, const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
