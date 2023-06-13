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

	byte red() const {
		return static_cast<byte>(clamp(r) * BYTE_MAX);
	}

	byte green() const {
		return static_cast<byte>(clamp(g) * BYTE_MAX);
	}

	byte blue() const {
		return static_cast<byte>(clamp(b) * BYTE_MAX);
	}

	const float& r = Vec3::x;
	const float& g = Vec3::y;
	const float& b = Vec3::z;
};

Color operator+(const Color& lhs, const Color& rhs) {
	return Color(operator+((Vec3)lhs, (Vec3)rhs));
}

inline float random_float() {
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	static std::mt19937 rand;
	return dist(rand);
}