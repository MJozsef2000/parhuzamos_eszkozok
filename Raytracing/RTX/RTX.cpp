#include "mathutils.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <omp.h>

#include <iostream>
//H�tt�r
color ray_color(const ray& r, const hittable& world) {
	hit_record rec; 
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //0.5 0.7 1.0 �gsz�n k�k
}

int main() {
	// K�p
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1920;
	const int image_height = 1080;
	const int samples_per_pixel = 100;

	// T�rgyak (g�mb�k) elhejez�se a vil�gban
	hittable_list world;
	// G�mb:                      point3(hol van a k�zepe),   milyen nagy a sugara
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;

	// Renderel�s
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
					pixel_color += ray_color(r, world);
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