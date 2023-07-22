#pragma once

#include "Ray.hpp"

class Camera {
public:
	Camera(const size_t width, const size_t height, double v_fov,
		Point lookfrom, Point lookat, Vec3 up) :
		m_vertical(), m_horizontal(), m_lowerleft()
	{
		const auto aspectRatio = (double)width / height;

		const auto theta = degrees_to_radians(v_fov);
		const auto h = tan(theta / 2);

		const auto viewportHeight = 2.0 * h;
		const auto viewportWidth = aspectRatio * viewportHeight;

		auto w = (lookfrom - lookat).unit();
		auto u = cross(up, w).unit();
		auto v = cross(w, u);

		m_origin = lookfrom;

		m_horizontal = viewportWidth * u;
		m_vertical =  viewportHeight * v;
		m_lowerleft = m_origin - m_horizontal / 2 - m_vertical / 2 - w;
	}

	Ray RayTo(double u, double v) const {
		return Ray(m_origin, u*m_horizontal + v*m_vertical + m_lowerleft - m_origin);
	}

private:
	
	Vec3 m_origin;
	Vec3 m_vertical;
	Vec3 m_horizontal;
	Vec3 m_lowerleft;
};