#include "mathutils.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <omp.h>

#include <iostream>
//Hatter
color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// Ha mar elfogyott a lepattanasok szama, nem vesszuk tovabb figyelembe ezt a fenyt
	if (depth <= 0)
		return color(0, 0, 0);
	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //0.5 0.7 1.0 egszin kek
}

int main() {
	// Kep
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1600;
	const int image_height = 900;
	const int samples_per_pixel = 1; //Elsimitas mintaszama
	const int max_depth = 5; // Ennyiszer pattanhat le egy fenysugar

	// Targyak (gombok) elhejezese a vilagban, anyag hozzarendelese
	hittable_list world;

	// Anyagok letrehozasa
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_matte = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_metal = make_shared<metal>(color(0.3, 0.3, 0.3));
	// Gombok letetele                    Ox    Oy      Oz     R        material
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_matte));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_metal));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_metal));

	world.add(make_shared<sphere>(point3(-0.5, 0.85, -1.0), 0.5, material_metal));
	world.add(make_shared<sphere>(point3(0.5, 0.85, -1.0), 0.5, material_matte));


	camera cam;
	// Rendereles
	const int PC_SIZE = image_height * image_width;
	color* pixel_colors = new color[PC_SIZE];
	std::cerr << "\rRENDERING: " << image_height << "/" << 0 << "\r" << std::flush;
	#pragma omp parallel 
	{
		#pragma omp for schedule(dynamic) collapse(2)
		for (int h = 0; h < image_height; h++) {
			for (int w = 0; w < image_width; w++) {
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					auto u = (w + random_double()) / (image_width - 1);
					auto v = (h + random_double()) / (image_height - 1);
					ray r = cam.get_ray(u, v);
					pixel_colors[(h * image_width) + w] += ray_color(r, world, max_depth);
				}
			}
		}
	}
	std::cerr << "\n";

	// Printing file out to .ppm format
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int h = image_height - 1; h >= 0; h--) {
		std::cerr << "\rWRITING: Maradek kepsorok szama: " << h << ' ' << std::flush;
		for (int w = 0; w < image_width; w++) {
			int iterator = (h * image_width) + w;
			write_color(std::cout, pixel_colors[iterator], samples_per_pixel);
		}
	}
	std::cerr << "\n Kesz!\n";
}
