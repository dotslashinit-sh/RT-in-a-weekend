#pragma once

#include "Vec3.hpp"

#include <cmath>
#include <random>

typedef uint8_t byte;

#define BYTE_MAX (uint16_t)0xFF
#define clamp(x) (x < 0.0) ? 0.0 : ((x > 1.0) ? 1.0 : x)

struct Color : public Vec3 {
	double length() const = delete;
	double lengthsq() const = delete;

	Color() : Vec3() { }
	Color(double red, double green, double blue) : Vec3(red, green, blue) { }

	Color(const Vec3& rhs) : Vec3(rhs) {}

	Color & operator=(const Vec3& rhs) {
		*this = Color(rhs);
		return *this;
	}

	Color& operator=(const Color& rhs) {
		this->x = rhs.r;
		this->y = rhs.g;
		this->z = rhs.b;
		return *this;
	}

	Color& operator+=(const Color& rhs) {
		*this = *this + rhs;
		return *this;
	}

	Color& operator/=(double val) {
		*this = *this / val;
		return *this;
	}

	Color& operator*=(double val) {
		*this = *this * val;
		return *this;
	}

	double& r = Vec3::x;
	double& g = Vec3::y;
	double& b = Vec3::z;
};

inline Color operator+(const Color& lhs, const Color& rhs) {
	return Color(operator+((Vec3)lhs, (Vec3)rhs));
}

inline double random_float(double min = 0.0, double max = 1.0) {
	static std::default_random_engine rand;
	std::uniform_real_distribution<double> dist(min, max);
	return dist(rand);
}

inline Vec3 random_vec3() {
	return Vec3(random_float(), random_float(), random_float());
}

inline Vec3 random_unit_vec() {
	return random_vec3().unit();
}

inline Point rand_point_in_unit_s() {
	while (true) {
		Point p = random_vec3();
		if (p.lengthsq() < 1)
			return p;
	}
}

inline Point rand_point_in_hemisphere(const Vec3& normal) {
	Point in_unit_s = rand_point_in_unit_s();
	if (dot(in_unit_s, normal) > 0.0) {
		return in_unit_s;
	}
	return -in_unit_s;
}

inline Vec3 reflect(const Vec3 & ray_dir, const Vec3 & normal) {
	return ray_dir - 2 * dot(ray_dir, normal) * normal;
}