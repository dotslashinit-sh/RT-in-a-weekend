#pragma once

#include <cmath>

struct Vec3;

// Used by Vec3::unit()
Vec3 operator/(const Vec3& vec, const double val);

struct Vec3 {
	double x;
	double y;
	double z;

	Vec3() : x(0.0), y(0.0), z(0.0) { }

	Vec3(double xval, double yval, double zval) :
		x(xval), y(yval), z(zval) { }

	const double length() const {
		return std::sqrt(this->lengthsq());
	}

	const double lengthsq() const {
		return x * x + y * y + z * z;
	}

	const Vec3 unit() const {
		return *this / this->length();
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	bool nearZero() const {
		double s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
	return lhs + (-rhs);
}

Vec3 operator*(const Vec3& vec, const double val) {
	return Vec3(vec.x * val, vec.y * val, vec.z * val);
}

Vec3 operator*(const double val, const Vec3& vec) {
	return vec * val;
}

Vec3 operator*(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

Vec3 operator/(const Vec3& vec, const double val) {
	return Vec3(vec.x / val, vec.y / val, vec.z / val);
}

double dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 & u, const Vec3 & v) {
	return Vec3(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

typedef Vec3 Point;
typedef Vec3 Direction;
typedef Vec3 Normal;