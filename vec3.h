
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

struct vec3
{
	vec3() 
	{
		this->e[0] = 0;
		this->e[1] = 0;
		this->e[2] = 0;
	}

	vec3(float e0, float e1, float e2)
	{
		this->e[0] = e0;
		this->e[1] = e1;
		this->e[2] = e2;
	}
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	float r() const { return e[0]; }
	float g() const { return e[1]; }
	float b() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const float t) {
		return *this *= 1/t;
	}

	float length() const {
		return sqrt(length_squared());
	}

	float length_squared() const {
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	}

	inline static vec3
    random()
	{
		return vec3(random_float(), 
					random_float(), 
					random_float());
	}

	inline static vec3
    random(float min, float max)
	{
		return vec3(random_float(min,max), 
					random_float(min,max), 
					random_float(min,max));
	}

    bool
    near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        const double s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    // Fields
	float e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
	return t * v;
}

inline vec3 operator/(const vec3 &v, float t) {
	return (1/t) * v;
}

inline float
dot(const vec3 &u, const vec3 &v)
{
	return u.e[0] * v.e[0]
		 + u.e[1] * v.e[1]
		 + u.e[2] * v.e[2];
}

inline vec3
cross(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

vec3
reflect(const vec3& v, const vec3& n)
{
    return v - 2*dot(v,n)*n;
}

vec3
refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
	float cos_theta = fmin(dot(-uv, n), 1.0f);
	vec3 r_out_perpendicular = etai_over_etat * (uv + cos_theta*n);
	vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perpendicular.length_squared())) * n;
	return r_out_perpendicular + r_out_parallel;
}

inline vec3 
unit_vector(vec3 v)
{
	return v / v.length();
}

// Uses a rejection method to find a random point in a unit sphere.
vec3
random_in_unit_sphere()
{
	while(true)
	{
		vec3 p = vec3::random(-1,1);
		if(p.length_squared() >= 1) continue;
		return p;
	}
}

// Generates true Lambertian samples of a unit sphere.
vec3
random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

// Generates random vectors in a uniform scatter direction.
vec3
random_in_hemisphere(const vec3& normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if(dot(in_unit_sphere, normal) > 0.0f) // In the same hemisphere as normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

vec3 random_in_unit_disk()
{
    while(true)
    {
        vec3 p = vec3(random_float(-1,1), random_float(-1,1), 0);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}

#endif
