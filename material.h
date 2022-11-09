#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtw.h"

struct hit_record;

struct material
{
	virtual bool
	scatter(const ray& r_in, const hit_record& rec, 
			color *attenuation, ray *scattered) const = 0;
};

struct lambertian : public material
{
	lambertian(const color& a)
	{
		this->albedo = a;
	}

	virtual bool
	scatter(const ray& r_in, const hit_record& rec, 
			color *attenuation, ray *scattered)
	const override
	{
		vec3 scatter_direction = rec.normal + random_unit_vector();

		// Catch edge case scatter direction
		if(scatter_direction.near_zero())
			scatter_direction = rec.normal;

		*scattered = ray(rec.p, scatter_direction);
		*attenuation = albedo;
		return true;
	}

	// Fields
	color albedo;
};


struct metal : public material
{
	metal(const color& a, float f)
	{
		this->albedo = a;
		this->fuzz = f < 1 ? f : 1;
	}

	virtual bool
	scatter(const ray& r_in, const hit_record& rec, 
			color *attenuation, ray *scattered)
	const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		*scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		*attenuation = albedo;
		return (dot(scattered->direction(), rec.normal) > 0);
	}

	// Fields
	color albedo;
	float fuzz;
};


struct dielectric : public material
{
	dielectric(double index_of_refraction)
	{
		this->ir = index_of_refraction;
	}

	virtual bool
	scatter(const ray& r_in, const hit_record& rec, 
			color *attenuation, ray *scattered)
	const override
	{
		*attenuation = color(1.0f, 1.0f, 1.0f);
		float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
        float sin_theta = sqrt(1.0f - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        vec3 direction;

        if(cannot_refract)
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

		*scattered = ray(rec.p, direction);
		return true;
	}

	// Fields
	float ir;

    // Uses Schlick's approximation for reflectance.
    static double
    reflectance(float cosine, float ref_idx)
    {
        float r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine), 5);
    }
};

#endif
