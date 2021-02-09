#include <rt/primmod/instance.h>

namespace rt {

Instance::Instance(Primitive* content)
{
    Total = Matrix::identity();
    TotalInverse = Total;
    primitive = content;
}

Primitive* Instance::content() {
    return primitive;
}

void Instance::reset() {
    Total = Matrix::identity();
    TotalInverse = Total;
}

void Instance::translate(const Vector& t) {
    Float4 v1(1,0,0,t.x);
    Float4 v2(0,1,0,t.y);
    Float4 v3(0,0,1,t.z);
    Float4 v4(0,0,0,1);

    Matrix m(v1, v2, v3, v4);
    appendToTotal(m);
}

void Instance::rotate(const Vector& nnaxis, float angle) {
    // 𝑅(r,𝜙) = 𝑀(𝑟)𝑅𝑥(𝜙)𝑀𝑇(𝑟)

    // Orthonormal basis with nnaxis=r as (r,s,t)
    Vector t,s,r = nnaxis.normalize();

    float minVal = min(abs(r.x), abs(r.y), abs(r.z));
    if(r.x == minVal)
        s = Vector(0.f, -r.z, r.y);
    else if(r.y == minVal)
        s = Vector(-r.z, 0.f, r.x);
    else if(r.z == minVal)
        s = Vector(-r.y, r.x, 0.f);

    s = s.normalize();

    assert(s != Vector::rep(0.f));
    assert(dot(s, r) == 0);

    t = cross(r, s);

    // Build the system with r,s,t
    Matrix M = Matrix::system(r, s, t);

    Matrix Rx;
    Rx[0] = Float4(1.f, 0.f, 0.f, 0.f);
    Rx[1] = Float4(0.f, cos(angle), -sin(angle), 0.f);
    Rx[2] = Float4(0.f, sin(angle), cos(angle), 0.f);
    Rx[3] = Float4(0.f, 0.f, 0.f, 1.f);

    Matrix transform = product(product(M, Rx), M.transpose());
    appendToTotal(transform);
}

void Instance::scale(float f) {
    this->scale(Vector(f, f, f));
}

void Instance::scale(const Vector& s) {
    Float4 sx(s.x, 0,0,0);
    Float4 sy(0, s.y, 0,0);
    Float4 sz(0, 0, s.z, 0);
    Float4 sw(0,0,0,1);

    Matrix m(sx, sy, sz, sw);
    appendToTotal(m);
}

void Instance::appendToTotal(Matrix& M)
{
    Total = product(M, Total);
    TotalInverse = Total.invert();
}

void Instance::setMaterial(Material* m) {
    primitive->setMaterial(m);
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {

    Point origin_3(TotalInverse * Float4(ray.o));
    Vector direction_3(TotalInverse * Float4(ray.d));

    // constrcut a new ray
    Ray r(origin_3, direction_3.normalize());

    Intersection Intersec = primitive->intersect(r);

    if (Intersec)
    {
        float length_ratio =  direction_3.length() / ray.d.length();
        float dist = Intersec.distance / length_ratio;

        if (dist > 0 && dist < previousBestDistance)
        {
            Point hitPoint = ray.getPoint(dist);
            Solid *solid = (Solid*)this;
            Vector normal = (TotalInverse.transpose() * Intersec.normal()).normalize();
            return Intersection(dist, ray, solid, normal, hitPoint);
        }
        else
            return Intersection::failure();
    }
    return Intersection::failure();
}

BBox Instance::getBounds() const {
    BBox bbox = primitive->getBounds();
    int n = 8;
    Float4 All_Points[n];

    // construct all points
    All_Points[0] = Float4(bbox.min);
    All_Points[1] = Float4(bbox.min.x, bbox.min.y, bbox.max.z, 1);
    All_Points[2] = Float4(bbox.min.x, bbox.max.y, bbox.min.z, 1);
    All_Points[3] = Float4(bbox.min.x, bbox.max.y, bbox.max.z, 1);
    All_Points[4] = Float4(bbox.max.x, bbox.min.y, bbox.min.z, 1);
    All_Points[5] = Float4(bbox.max.x, bbox.min.y, bbox.max.z, 1);
    All_Points[6] = Float4(bbox.max.x, bbox.max.y, bbox.min.z, 1);
    All_Points[7] = Float4(bbox.max.x, bbox.max.y, bbox.max.z, 1);

    for (int i = 0; i < n; ++i)
        All_Points[i] = Total * All_Points[i];

    Point min = Point(All_Points[0]);
    Point max = Point(All_Points[7]);

    for (int i = 0; i < 8; ++i)
    {
        // get min
        if (min.x > All_Points[i].x)
            min.x = All_Points[i].x;
        if (min.y > All_Points[i].y)
            min.y = All_Points[i].y;
        if (min.z > All_Points[i].z)
            min.z = All_Points[i].z;
        // get max
        if (max.x < All_Points[i].x)
            max.x = All_Points[i].x;
        if (max.y < All_Points[i].y)
             max.y = All_Points[i].y;
        if (max.z < All_Points[i].z)
            max.z = All_Points[i].z;
    }
    return BBox(min, max);
}

    float Instance::getArea() const {
        return 0;
    }
}
