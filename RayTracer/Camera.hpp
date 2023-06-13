#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

class Camera {
public:
	Camera(const Point pos, const size_t width, const size_t height, float focalLength) : 
		m_width(width), m_height(height), m_position(pos),
		m_vertical(), m_horizontal(), m_lowerleft()
	{
		auto aspectRatio = (float)m_width / m_height;
		auto viewportHeight = 2.0f;
		auto viewportWidth = aspectRatio * viewportHeight;

		m_vertical = Vec3(0.0f, viewportHeight, 0.0f);
		m_horizontal = Vec3(viewportWidth, 0.0f, 0.0f);
		m_lowerleft = m_position - m_horizontal / 2 - m_vertical / 2 - Vec3(0.0f, 0.0f, focalLength);
	}

	Ray RayTo(float u, float v) const {
		return Ray(m_position, u*m_horizontal + v*m_vertical + m_lowerleft - m_position);
	}

private:
	const size_t m_width;
	const size_t m_height;
	
	const Vec3 m_position;
	Vec3 m_vertical;
	Vec3 m_horizontal;
	Vec3 m_lowerleft;
};