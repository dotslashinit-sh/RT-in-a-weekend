#pragma once
#include "Ray.hpp"
#include "Utils.hpp"

#include <cmath>
#include <vector>
#include <memory>
#include <functional>

using std::vector;
using std::shared_ptr, std::make_shared;
using std::sqrt;

class Material;
typedef shared_ptr<Material> MaterialPtr;

struct HitRecord {
	HitRecord() = default;

	HitRecord(const Ray & ray, double t_val, Normal outwardNorm, MaterialPtr material) :
		t(t_val),
		point(ray.at(t_val)),
		normal(outwardNorm.unit()),
		isFrontFace(dot(ray.direction(), outwardNorm.unit()) < 0.0),
		mat(material)
	{
		normal = isFrontFace ? normal : -normal;
	}

	double t = 0.0;
	bool isFrontFace = false;
	Point point = Vec3();
	Normal normal = Vec3();
	MaterialPtr mat;

	void Set(const Ray & ray, double t_val, const Vec3 & outwardNorm, MaterialPtr m) {
		*this = HitRecord(ray, t_val, outwardNorm, m);
	}
};

class Hittable abstract {
public:
	virtual bool isHit(const Ray& r, HitRecord & rec, double tmin, double tmax) abstract;
};

class Sphere : public Hittable {
public:

	Sphere() = delete;
	Sphere(const Vec3& center, double radius, MaterialPtr mat) : m_center(center), m_radius(radius), m_mat(mat) { }

	bool isHit(const Ray& r, HitRecord & rec, double tmin, double tmax) override {
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

		//rec.t = root;
		//rec.point = r.at(root);
		//rec.normal = (rec.point - m_center) / m_radius;
		rec.Set(r, root, rec.point- m_center, m_mat);
		return true;
	}

private:
	Point m_center;
	double m_radius;
	MaterialPtr m_mat;
};

class World : public Hittable {
public:
	World() : m_objects() {}

	void AddSphere(Point center, double radius, MaterialPtr mat) {
		m_objects.push_back(make_shared<Sphere>(center, radius, mat));
	}

	bool isHit(const Ray & r, HitRecord & rec, double tmin, double tmax) override {
		HitRecord temprec;
		double closest = tmax;
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