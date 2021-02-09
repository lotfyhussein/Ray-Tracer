#include <rt/integrators/casting.h>
#include <rt/intersection.h>
#include <rt/world.h>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {

    Intersection Intsec = this->world->scene->intersect(ray);
    if (Intsec)
    {
        Vector normal = Intsec.normal().normalize();
        Vector dir = ray.d.normalize();
        return RGBColor::rep(-dot(normal, dir));

    }

    return RGBColor(0,0,0);
}

}
