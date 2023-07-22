#pragma once

#include "Ray.hpp"

class Camera {
public:
	Camera(const size_t width, const size_t height, double v_fov,
		Point lookfrom, Point lookat, Vec3 up, double focus_dist, double aperture) :
		m_vertical(), m_horizontal(), m_lowerleft(),
		m_u(), m_v(), m_w(), m_lensRadius(aperture/2)
	{
		const auto aspectRatio = (double)width / height;
		const auto theta = degrees_to_radians(v_fov);
		const auto h = tan(theta / 2);

		const auto viewportHeight = 2.0 * h;
		const auto viewportWidth = aspectRatio * viewportHeight;

		m_w = (lookfrom - lookat).unit();
		m_u = cross(up, m_w).unit();
		m_v = cross(m_w, m_u);

		m_origin = lookfrom;

		m_horizontal = focus_dist * viewportWidth * m_u;
		m_vertical =  focus_dist * viewportHeight * m_v;
		m_lowerleft = m_origin - m_horizontal / 2 - m_vertical / 2 - focus_dist*m_w;
	}

	Ray RayTo(double u, double v) const {
		Vec3 rd = m_lensRadius * rand_point_in_unit_disk();
		Vec3 offset = m_u * rd.x + m_v * rd.y;

		return Ray(m_origin + offset, u*m_horizontal + v*m_vertical + m_lowerleft - m_origin - offset);
	}

private:
	
	Vec3 m_origin;
	Vec3 m_vertical;
	Vec3 m_horizontal;
	Vec3 m_lowerleft;

	Vec3 m_u, m_v, m_w;
	double m_aspectRatio;
	double m_lensRadius;
};