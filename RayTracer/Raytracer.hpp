#pragma once

#include "Utils.hpp"
#include "Ray.hpp"
#include "hittable.hpp"
#include "Camera.hpp"

#include <vector>
#include <optional>
#include <iostream>
#include <iomanip>
#include <limits>

class RayTracer {
public:
	RayTracer() = delete;

	RayTracer(const size_t width, const size_t height) : 
		m_width(width), m_height(height), m_aspectRatio((float)width/height),
		m_data(width * height * 3, 0x00),
	    m_vertical(), m_horizontal(), m_lowerleft(), m_origin() {

		const float viewport_height = 2.0f;
		const float viewport_width = m_aspectRatio * viewport_height;
		const float focal_length = 1.0f;

		m_horizontal = Vec3(viewport_width, 0.0f, 0.0f);
		m_vertical = Vec3(0.0f, viewport_height, 0.0f);
		m_lowerleft = m_origin - m_horizontal / 2 - m_vertical / 2 - Vec3(0.0f, 0.0f, focal_length);
	}

	const void* GetBitmap() const {
		return m_data.data();
	}

	void Run() {
		static const Vec3 origin = Vec3(0.0f, 0.0f, 0.0f);
		World world;

		constexpr int SAMPLE_COUNT = 4;

		world.AddSphere(Point(0.0f, 0.0f, -1.0f), 0.5f);
		world.AddSphere(Point(0.0f, -100.5f, -1.0f), 100.0f);

		Camera camera(Point(0.0f, 0.0f, 0.0f), m_width, m_height, 1.0f);

		constexpr int MAX_REFLECT = 5;
		auto scale = 1.0f / SAMPLE_COUNT;

		for (size_t j = 0; j < m_height; ++j) {

			for (size_t i = 0; i < m_width; ++i) {
				Color pixelColor(0.1f, 0.1f, 0.1f);

				for (size_t k = 0; k < SAMPLE_COUNT; ++k) {
					float u = ((float)i + random_float()) / (m_width - 1);
					float v = ((float)j + random_float()) / (m_height - 1);

					Ray ray = camera.RayTo(u, v);
					pixelColor += ColorAt(ray, world, MAX_REFLECT);
				}

				// Anti-aliasing
				pixelColor *= scale;

				// Gamma correction
				pixelColor.r = sqrt(pixelColor.r/* * scale*/);
				pixelColor.g = sqrt(pixelColor.g/* * scale*/);
				pixelColor.b = sqrt(pixelColor.b /** scale*/);

				size_t index = (i + m_width * j) * 3;
				m_data[index] = static_cast<byte>(clamp(pixelColor.r) * BYTE_MAX);
				m_data[index + 1] = static_cast<byte>(clamp(pixelColor.g) * BYTE_MAX);
				m_data[index + 2] = static_cast<byte>(clamp(pixelColor.b) * BYTE_MAX);
			}
			float percent = (float)j / (m_width - 1) * 100;
			std::cout << "Completed: " << std::setprecision(4) << std::setw(7) << percent << "%\r";
		}
	}

private:

	const Color ColorAt(const Ray& ray, Hittable & world, int depth) {
		if (depth <= 0) {
			return Color(0.0, 0.0, 0.0); // Absolute black
		}

		static constexpr float F_INFINITE = std::numeric_limits<float>::infinity();

		HitRecord rec;
		HitRecord hitRecord;
		if (world.isHit(ray, hitRecord, 0.0001f, F_INFINITE)) {
			Point target = hitRecord.point + hitRecord.normal + rand_point_in_hemisphere(hitRecord.normal);
			return 0.5 * ColorAt(Ray(hitRecord.point, target - hitRecord.point), world, depth-1);
		}
		return SkyColor(ray);
	}

	const Color SkyColor(const Ray & r) const {
		float t = (1.0f + r.direction().unit().y) * 0.5f;
		return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
	}

private:
	const size_t m_width;
	const size_t m_height;
	const float m_aspectRatio;
	std::vector<byte> m_data;

	// Not const becuase its values are set in the constructor body.
	Vec3 m_origin;
	Vec3 m_vertical;
	Vec3 m_horizontal;
	Vec3 m_lowerleft;
};