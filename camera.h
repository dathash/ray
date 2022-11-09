#ifndef CAMERA_H
#define CAMERA_H

#include "rtw.h"

class camera
{
public:
	camera(point3 lookfrom,
           point3 lookat,
           vec3 up,
           float vfov, // vertical field-of-view in degrees
           float aspect_ratio,
           float aperture,
           float focus_dist
          )
	{
        float theta = degrees_to_radians(vfov);
        float h = tan(theta/2);

		float viewport_height = 2.0f * h;
		float viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up, w));
        v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
	}

	ray
	get_ray(float s, float t) const
	{
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
		return ray(
                origin + offset, 
                lower_left_corner + s*horizontal + t*vertical - origin - offset
               );
	}

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif
