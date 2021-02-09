#include <rt/solids/striangle.h>
#include <rt/intersection.h>

namespace rt {

SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
{
    this->texMapper = texMapper;
    this->material = material;
    
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    Vector v21 = (v2 - v1).normalize();
    Vector v31 = (v3 - v1).normalize();
    OriginalNormal = cross(v21, v31).normalize();
    this->n1 = normals[0];
    this->n2 = normals[1];
    this->n3 = normals[2];
}

SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
{
    this->texMapper = texMapper;
    this->material = material;
    
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    Vector v21 = (v2 - v1).normalize();
    Vector v31 = (v3 - v1).normalize();
    OriginalNormal = cross(v21, v31).normalize();
    this->n1 = n1;
    this->n2 = n2;
    this->n3 = n3;
}

Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const {

    if (dot(ray.d, this->OriginalNormal) == 0.0) return Intersection::failure();
    float t = - dot(ray.o - v1, OriginalNormal) / dot(ray.d, this->OriginalNormal);

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
    Vector n = n1 * lambda1 + n2 * lambda2 + n3 * lambda3;
    return Intersection(t, ray, this, n, Point(lambda1, lambda2, lambda3));
}

}