#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

void
write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{
	float r = pixel_color.r();
	float g = pixel_color.g();
	float b = pixel_color.b();

	float scale = 1.0f / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0, 255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << ' '
	    << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}

#endif
