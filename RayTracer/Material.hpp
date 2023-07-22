#pragma once

#include "Utils.hpp"
#include "Ray.hpp"
#include "hittable.hpp"

#include <memory>

class Material {
public:
	virtual bool Scatter(const Ray& ray_in, const HitRecord & rec, Color& attenuation, Ray& ray_out) = 0;
};

class Lambertian final : public Material {
public:
	Lambertian(const Color & albedo) : m_albedo(albedo) {}

	virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& ray_out) override {
		auto scatter_dir = rec.normal + random_unit_vec();
		if (scatter_dir.nearZero()) {
			scatter_dir = rec.normal;
		}
		ray_out = Ray(rec.point, scatter_dir);
		attenuation = m_albedo;
		return true;
	}

private:
	Color m_albedo;
};

class Metal final : public Material {
public:
	Metal(const Color & albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray & ray_in, const HitRecord & rec, Color & attenuation, Ray & ray_out) override {
		Vec3 reflected = reflect(ray_in.direction(), rec.normal);
		ray_out = Ray(rec.point, reflected + m_fuzz*rand_point_in_unit_s());
		attenuation = m_albedo;
		return dot(ray_out.direction(), rec.normal) > 0.0;
	}

private:
	Color m_albedo;

	float m_fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double ir) : m_ir(ir) {}

	bool Scatter(const Ray & ray, const HitRecord & record, Color & attenuation, Ray & ray_out) override {
		attenuation = Color(1.0, 1.0, 1.0);
		double ir = record.isFrontFace ? (1.0 / m_ir) : m_ir;
		auto r_dir = ray.direction().unit();

		auto cthetha = fmin(dot(-r_dir, record.normal.unit()), 1.0);
		auto sthetha = sqrt(1.0 - cthetha * cthetha);

		Vec3 direction;
		bool cannot_refract = sthetha * ir > 1.0;
		if (cannot_refract || reflectance(cthetha, ir) > random_double()) {
			direction = reflect(r_dir, record.normal);
		}
		else {
			direction = refract(r_dir, record.normal, ir);
		}

		ray_out = Ray(record.point, direction);
		return true;
	}

private:
	double m_ir;

	static double reflectance(double cosine, double ir) {
		auto r0 = (1 - ir) / (1 + ir);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

typedef std::shared_ptr<Material> MaterialPtr;