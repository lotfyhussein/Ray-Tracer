#include <rt/integrators/raytrace.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/materials/flatmaterial.h>

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {

    Intersection Intersec = world->scene->intersect(ray);
    Vector outDir = -ray.d.normalize();
    RGBColor emission, intensity, reflectance, irradiance, color = RGBColor::rep(0.0f);
    Point texPoint;

    if (Intersec){
        texPoint = Intersec.solid->texMapper->getCoords(Intersec);
        emission = Intersec.solid->material->getEmission(texPoint, Intersec.normal(), outDir);
        color = color + emission;

        for(std::vector<Primitive *>::size_type i = 0; i != world->light.size(); i++){
            // light hit
            LightHit lightHit = world->light[i]->getLightHit(Intersec.hitPoint());

            // create shadow ray with an offset to avoid self intersection
						Vector inDir = lightHit.direction.normalize();
						Ray shadowRay(Intersec.hitPoint() + Intersec.normal() * offset, inDir);

						// make sure that the shadow ray and the primary ray leave the surface on the same side (means the primitive surface is on the same side with light)
            if(dot(Intersec.normal(), shadowRay.d) > 0.0f) {
                Intersection obstacleIntersec = world->scene->intersect(shadowRay, lightHit.distance);
                // if there is no intersection with another object towards the light source, add to color
                if(!obstacleIntersec) {
                    intensity = world->light[i]->getIntensity(lightHit);
                    reflectance = Intersec.solid->material->getReflectance(texPoint, Intersec.normal(), outDir, -inDir);
                    irradiance = intensity * reflectance;
                    color = color + irradiance;
                }
            }
        }
    }
    return color;

}

}
