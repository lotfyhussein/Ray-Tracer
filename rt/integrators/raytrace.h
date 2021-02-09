#ifndef CG1RAYTRACER_INTEGRATORS_RAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RAYTRACING_HEADER

#include <rt/integrators/integrator.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <core/color.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>

namespace rt {
class World;
class Ray;
class RGBColor;

class RayTracingIntegrator : public Integrator {
public:
    RayTracingIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const;

private:
    float offset =  0.0001f;
};

}

#endif
