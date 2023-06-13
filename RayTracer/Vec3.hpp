#pragma once

#include <cmath>

struct Vec3;

// Used by Vec3::unit()
Vec3 operator/(const Vec3& vec, const float val);

struct Vec3 {
	float x;
	float y;
	float z;

	Vec3() : x(0.0f), y(0.0f), z(0.0f) { }

	Vec3(float xval, float yval, float zval) :
		x(xval), y(yval), z(zval) { }

	const float length() const {
		return std::sqrt(this->lengthsq());
	}

	const float lengthsq() const {
		return x * x + y * y + z * z;
	}

	const Vec3 unit() const {
		return *this / this->length();
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
	return lhs + (-rhs);
}

Vec3 operator*(const Vec3& vec, const float val) {
	return Vec3(vec.x * val, vec.y * val, vec.z * val);
}

Vec3 operator*(const float val, const Vec3& vec) {
	return vec * val;
}

Vec3 operator/(const Vec3& vec, const float val) {
	return vec * (1.0f / val);
}

float dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

typedef Vec3 Point;
typedef Vec3 Direction;
typedef Vec3 Normal;