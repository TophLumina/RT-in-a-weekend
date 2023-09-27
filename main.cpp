#include "rtweekend.h"

#include "color.h"
#include "material.h"
#include "camera.h"

int main()
{
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.6));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right =  make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    
    // Sphere with negative radius can invert the normal and two spheres together froms a bubble
    world.add(make_shared<sphere>(point3(-1.0, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0, -1), -0.4, material_left));
    
    world.add(make_shared<sphere>(point3(1.0, 0, -1), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samplers_per_pixel = 256;
    cam.ray_gen_probability = 0.9;

    cam.vfov = 20;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    clog << "Samplers: " << cam.samplers_per_pixel << '\n';

    cam.render(world);
}