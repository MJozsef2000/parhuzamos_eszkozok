#include "mathutils.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <omp.h>

#include <iostream>
//Háttér
color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// Ha már elfogyott a lepattanások száma, nem vesszük tovább figyelembe ezt a fényt
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
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //0.5 0.7 1.0 Égszín kék
}

int main() {
	// Kép
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1600;
	const int image_height = 900;
	const int samples_per_pixel = 100;
	const int max_depth = 5; // Ennyiszer pattanhat le egy fénysugár

	// Tárgyak (gömbök) elhejezése a világban, anyag hozzárendelése
	hittable_list world;

	// Anyagok létrehozása
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_matte = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_metal = make_shared<metal>(color(0.3, 0.3, 0.3));
	// Gömbök letétele                    Ox    Oy      Oz     R        material
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_matte));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_metal));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_metal));

	world.add(make_shared<sphere>(point3(-0.5, 0.85, -1.0), 0.5, material_metal));
	world.add(make_shared<sphere>(point3(0.5, 0.85, -1.0), 0.5, material_matte));


	camera cam;

	// Renderelés
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; j--) {
		std::cerr << "\rMaradek kepsorok szama: " << j << ' ' << std::flush;
#		pragma omp parallel
		{
#pragma omp for ordered schedule(dynamic)
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					auto u = (i + random_double()) / (image_width - 1);
					auto v = (j + random_double()) / (image_height - 1);
					ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
#pragma omp ordered
				{
					write_color(std::cout, pixel_color, samples_per_pixel);
				}
			}
		}
	}
	std::cerr << "\n Kesz!\n";
}