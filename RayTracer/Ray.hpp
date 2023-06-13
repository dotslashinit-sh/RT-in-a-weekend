#pragma once

#include "Vec3.hpp"

class Ray {
public:

	Ray(const Vec3 & origin, const Vec3 & direction) : m_origin(origin), m_dir(direction) { }

	const Vec3 at(float t) const {
		return m_origin + t * m_dir;
	}

	const Vec3& origin() const {
		return m_origin;
	}

	const Vec3& direction() const {
		return m_dir;
	}

private:
	Vec3 m_origin;
	Vec3 m_dir;
};