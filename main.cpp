#include <iostream>
#include "color.h"
#include "ray.h"
#include "vec3.h"
using namespace std;

bool hit_sphere(const point3& center, double radius, const ray& r) {
    auto c_minus_q = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2 * dot(r.direction(), c_minus_q);
    auto c = dot(c_minus_q, c_minus_q) - radius * radius;
    auto d = b * b - 4 * a * c;
    return d >= 0;
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.x() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 960;

    int image_height = int(image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) / 2;

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        clog << "\rLines remaining: " << (image_height - j) << ' ' << flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(cout, pixel_color);
        }
    }

    std::clog << "\rDone!\n";
}