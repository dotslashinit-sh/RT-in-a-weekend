#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"
#include "Utils.hpp"

#include <cmath>
#include <vector>
#include <memory>
#include <functional>

using std::vector;
using std::shared_ptr, std::make_shared;
using std::min, std::max, std::sqrt;

struct HitRecord {
	HitRecord() = default;

	HitRecord(const Ray & ray, float t_val, Normal outwardNorm) :
		t(t_val),
		point(ray.at(t_val)),
		normal(outwardNorm.unit()),
		isFrontFace(dot(ray.direction(), outwardNorm) < 0.0)
	{
		normal = isFrontFace ? normal : -normal;
	}

	float t = 0.0f;
	bool isFrontFace = false;
	Point point = Vec3();
	Normal normal = Vec3();

	void Set(const Ray & ray, float t_val, const Vec3 & outwardNorm) {
		*this = HitRecord(ray, t_val, outwardNorm);
	}
};

class Hittable abstract {
public:
	virtual bool isHit(Ray& r, HitRecord & rec, float tmin, float tmax) abstract;
};

class Sphere : public Hittable {
public:

	Sphere() = delete;
	Sphere(const Vec3& center, float radius) : m_center(center), m_radius(radius) { }

	bool isHit(Ray& r, HitRecord & rec, float tmin, float tmax) override {
		auto oc = r.origin() - m_center;
		auto a = r.direction().lengthsq();
		auto half_b = dot(r.direction(), oc);
		auto c = oc.lengthsq() - m_radius * m_radius;

		auto d = half_b * half_b - a * c;
		bool hit = d > 0;
		if (!hit) {
			return false;
		}

		auto root = (- half_b - sqrt(d)) / a;
		if (root < tmin || tmax < root) {
			// Check if the other root is possible.
			root = (-half_b + sqrt(d)) / a;
			if (root < tmin || tmax < root) {
				return false;
			}
		}

		rec.t = root;
		rec.point = r.at(root);
		rec.normal = (rec.point - m_center).unit();

		return true;
	}

private:
	Point m_center;
	float m_radius;
};

class World : public Hittable {
public:
	World() : m_objects() {}

	void AddSphere(Point center, float radius) {
		m_objects.push_back(make_shared<Sphere>(center, radius));
	}

	bool isHit(Ray & r, HitRecord & rec, float tmin, float tmax) override {
		HitRecord temprec;
		float closest = tmax;
		bool hit = false;
		for (auto& object : m_objects) {
			if (object->isHit(r, temprec, tmin, closest)) {
				hit = true;
				closest = temprec.t;
			}
		}
		if (hit) {
			rec = temprec;
		}
		return hit;
	}

	void Clear() {
		m_objects.clear();
	}

private:
	vector<shared_ptr<Hittable>> m_objects;
};