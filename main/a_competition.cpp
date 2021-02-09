
#include <core/image.h>
#include <core/color.h>
#include <core/random.h>

#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/casting.h>

#include <rt/groups/simplegroup.h>
#include <rt/groups/bvh.h>

#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>

#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/solids/triangle.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/perlin.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/materials/glass.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>
#include <rt/materials/cooktorrance.h>
#include <rt/materials/mirror.h>
#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>
#include <rt/lights/directional.h>
#include <rt/primmod/bmap.h>
#include <algorithm>

using namespace rt;


void a_competition() {
    Image img(400, 230);


    BVH* scene = new BVH();


    //Front Pyramid
    MatLib* pyramid_matlib = new MatLib;
    Texture* greytex = new ConstantTexture(RGBColor::rep(0.2f));
    ImageTexture* pyramid_tex = new ImageTexture("models/stones.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    Material* pyramid = new LambertianMaterial(pyramid_tex,greytex);
    pyramid_matlib->insert(std::pair<std::string, Material*>("None", pyramid));
    pyramid_matlib->insert(std::pair<std::string, Material*>("initialShadingGroup.002", pyramid));
    loadOBJ(scene, "models/", "pyramid_front.obj",pyramid_matlib);


    //Back Pyramid
    loadOBJ(scene, "models/", "pyramid_back.obj",pyramid_matlib);


    //Statue
    MatLib* statue_matlib = new MatLib;
    Material* glass_mat = new GlassMaterial(2.f);

    statue_matlib->insert(std::pair<std::string, Material*>("Default_OBJ", glass_mat));
    statue_matlib->insert(std::pair<std::string, Material*>("initialShadingGroup.002", glass_mat));
    statue_matlib->insert(std::pair<std::string, Material*>("None", glass_mat));
    loadOBJ(scene, "models/", "statues.obj",statue_matlib);
  


    World world;
    world.scene = scene;

    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));
    Texture* yellowtex = new ConstantTexture(RGBColor(0.467f, 0.14f, 0.09f));


    //Back wall
    float scale =0.008f;
    ImageTexture* wall = new ImageTexture("models/n-final.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    Material* wall_mat = new LambertianMaterial(wall,greytex);
    scene->add(new Quad(Point(-130.0f, 000.f, 300.f)*scale, Vector(000.f, 300.f, 000.f)*scale, Vector(300.f, 000.f, 000.f)*scale, nullptr, wall_mat));


    //Floor
    ImageTexture* bumptex = new ImageTexture("models/stones_bump.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    Material* grey_cook = new CookTorrance(greytex,whitetex, 0.6f, 0.4f, 0.1f);

    Material* fuzzy_mat = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.01f);
    CombineMaterial* combined = new CombineMaterial();
    combined->add(grey_cook, 0.5f);
    combined->add(fuzzy_mat, 0.4f);
    combined->add(glass_mat, 0.1f);
    scene->add(
        new BumpMapper(
          new Triangle(Point(-1.5f, 0.f, 0.f) * 2.0f, Point(-1.5f, 0.f, 3.0f) * 2.0f, Point(1.5f, 0.f, 0.f) * 3.0f, nullptr, combined),
            bumptex, Point(0.0f,0.0f,0.0f) * 4.5f, Point(0.0f, 3.0f, 0.0f) * 4.5f, Point(4.0f, 0.0f, 0.0f) * 4.5f, 1.0f)
            );


    // Light
    RGBColor yellow_clr =  RGBColor(0.9f, 0.4f, 0.1f);
    RGBColor blue_clr = RGBColor(0.035f, 0.252f, 1.0f);
    RGBColor red_clr = RGBColor(1.0f, 0.02f, 0.02f);

    world.light.push_back(new PointLight(Point(0.35f, 0.18f, 0.6f),  RGBColor(0.467f, 0.14f, 0.09f) * 0.25)); 
    world.light.push_back(new PointLight(Point(0.0f, 0.4f, 0.5f), red_clr * 0.5f));     
    world.light.push_back(new PointLight(Point(0.5f, 0.8f, 0.8f), yellow_clr * 1.0f)); 
    world.light.push_back(new PointLight(Point(0.5f, 0.8f, 1.6f), yellow_clr * 1.0f)); 
    world.light.push_back(new SpotLight(Point(0.52f, 0.35f, 0.90f), Vector(-0.3f, -1.0f, -0.8f),  pi/25, 1.0f, blue_clr * 1.8f)); 
    world.light.push_back(new DirectionalLight(Vector(0.2f ,-0.5f , 1.0f).normalize(), RGBColor(1.0f, 1.0f, 1.0f) * 0.15f)); 
    world.light.push_back(new DirectionalLight(Vector(-0.2f ,-0.5f , 1.0f).normalize(), yellow_clr * 1.f));




    RecursiveRayTracingIntegrator integrator(&world);
	scene->rebuildIndex();


    PerspectiveCamera* cam = new PerspectiveCamera(Point(0.2f, 0.2f, -0.15f), Vector(0, 0, 1), Vector(0, 1, 0), pi/3, pi/4);
    Renderer engine2(cam, &integrator);
    engine2.setSamples(20);
    engine2.render(img);
    img.writePNG("comp400x230.png");
}