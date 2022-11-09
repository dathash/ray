#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

struct sphere : public hittable
{
	sphere() {}
	sphere(point3 cen, float r, shared_ptr<material> m)
	{
		this->center = cen;
		this->radius = r;
		this->mat_ptr = m;
	}

	virtual bool hit(
		const ray& r, float t_min, float t_max, hit_record *rec) const override;

	point3 center;
	float radius;
	shared_ptr<material> mat_ptr;
};

// Applies the sphere-line collision algorithm to determine if a ray hits a sphere
bool 
sphere::hit(const ray& r, float t_min, float t_max, hit_record *rec) const
{
	vec3 oc = r.origin() - center;
	float a = r.direction().length_squared();
	float half_b = dot(oc, r.direction());
	float c = oc.length_squared() - radius*radius;

	float discriminant = half_b*half_b - a*c;
	if(discriminant < 0)
	{
		return false;
	}
	float sqrtd = sqrt(discriminant);

	float root = (-half_b - sqrtd) / a;
	if(root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if(root < t_min || t_max < root)
		{
			return false;
		}
	}

	rec->t = root;
	rec->p = r.at(rec->t);
	vec3 outward_normal = (rec->p - center) / radius;
	rec->set_face_normal(r, outward_normal);
	rec->mat_ptr = mat_ptr;

	return true;
}

#endif
