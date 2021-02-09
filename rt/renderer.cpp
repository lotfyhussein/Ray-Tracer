#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <rt/cameras/orthographic.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/integrator.h>
#include <core/random.h>


namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

void Renderer::render(Image& img) {
    for (uint y = 0; y < img.height(); ++y) {
        for (uint x = 0; x < img.width(); ++x) {
            if (x%10 ==0)
                std::cout << "pixel:" << x << " " << y << std::endl;
            if (samples == 1) {
                float cameraX = (2.0f * float(x + 0.5f) / float(img.width()) - 1.f);
                float cameraY = 1.f - 2.0 * float(y + 0.5f) / float(img.height());
                Ray r = this->cam->getPrimaryRay(cameraX, cameraY);
                img(x, y) = this->integrator->getRadiance(r);
            } else {
                RGBColor totalRadiance = RGBColor::rep(0.f);
                for (uint s = 0; s < samples; s++) {
                    float cameraX = (2.0f * float(x + random()) / float(img.width()) - 1.f);
                    float cameraY = 1.f - 2.0 * float(y + random()) / float(img.height());
                    Ray r = this->cam->getPrimaryRay(cameraX, cameraY);
                    totalRadiance = totalRadiance + this->integrator->getRadiance(r);
                }
                img(x, y) = totalRadiance / samples;
            }
        }
    }
}

}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
    for (uint y = 0; y < img.height(); ++y)
        for (uint x = 0; x < img.width(); ++x)
            img(x, y) = a1computeColor(x, y, img.width(), img.height());
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {

    float ImageAspectRatio = img.width()/img.height();

    for (uint y = 0; y < img.height(); ++y) {
        for (uint x = 0; x < img.width(); ++x) {
            float cameraX = (2.0f * float(x + 0.5f) / float(img.width()) - 1.f);
            float cameraY = 1.f - 2.0 * float(y + 0.5f) / float(img.height());
            Ray r = this->cam->getPrimaryRay(cameraX, cameraY);
            img(x, y) = a2computeColor(r);
        }
    }

}

}
