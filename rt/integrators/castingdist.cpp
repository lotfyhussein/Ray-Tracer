#include <rt/integrators/castingdist.h>
#include <rt/world.h>
#include <math.h>     
namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    this->nearColor = nearColor;
    this->nearDist = nearDist; 
    this->farColor = farColor;
    this->farDist = farDist; 
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    Intersection Intsec = this->world->scene->intersect(ray);
    if (Intsec)
    {
        Vector normal = Intsec.normal().normalize();
        Vector dir = ray.d.normalize();
        RGBColor interpolated_color;
        if (Intsec.distance >= farDist)
            interpolated_color = farColor;
        else if (Intsec.distance <= nearDist)
            interpolated_color = nearColor;
        else
            interpolated_color = nearColor + ((farColor-nearColor)/(farDist-nearDist)) *  (Intsec.distance - nearDist);

        return RGBColor(interpolated_color *dot(-dir, normal));

    }

    return RGBColor(0,0,0);
}

}
