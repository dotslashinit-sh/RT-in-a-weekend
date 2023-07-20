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
	Metal(const Color & albedo) : m_albedo(albedo) {}

	bool Scatter(const Ray & ray_in, const HitRecord & rec, Color & attenuation, Ray & ray_out) override {
		Vec3 reflected = reflect(ray_in.direction(), rec.normal);
		ray_out = Ray(rec.point, reflected);
		attenuation = m_albedo;
		return dot(ray_out.direction(), rec.normal) > 0.0;
	}

private:
	Color m_albedo;
};

typedef std::shared_ptr<Material> MaterialPtr;