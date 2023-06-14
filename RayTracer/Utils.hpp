#pragma once

#include "Vec3.hpp"

#include <cmath>
#include <random>

typedef uint8_t byte;

#define BYTE_MAX (uint16_t)0xFF
#define clamp(x) (x < 0.0f) ? 0.0f : ((x > 1.0f) ? 1.0f : x)

struct Color : public Vec3 {
	float length() const = delete;
	float lengthsq() const = delete;

	Color(float red, float green, float blue) : Vec3(red, green, blue) { }

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

	Color& operator/=(float val) {
		*this = *this / val;
		return *this;
	}

	Color& operator*=(float val) {
		*this = *this * val;
		return *this;
	}

	float& r = Vec3::x;
	float& g = Vec3::y;
	float& b = Vec3::z;
};

inline Color operator+(const Color& lhs, const Color& rhs) {
	return Color(operator+((Vec3)lhs, (Vec3)rhs));
}

inline float random_float(float min = 0.0f, float max = 1.0f) {
	static std::mt19937 rand;
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rand);
}

inline Vec3 random_vec3(float min, float max) {
	return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

inline Point rand_point_in_unit_s() {
	while (true) {
		Point p = random_vec3(-1.0f, 1.0f);
		if (p.lengthsq() >= 1) continue;
		return p.unit();
	}
}

inline Point rand_point_in_hemisphere(const Vec3& normal) {
	Point in_unit_s = rand_point_in_unit_s();
	if (dot(in_unit_s, normal) > 0.0f) {
		return in_unit_s;
	}
	return -in_unit_s;
}