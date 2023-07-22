#pragma once

#include "Utils.hpp"
#include "Ray.hpp"
#include "hittable.hpp"
#include "Camera.hpp"
#include "Material.hpp"

#include <vector>
#include <optional>
#include <iostream>
#include <iomanip>
#include <limits>



class RayTracer {
public:
	RayTracer() = delete;

	RayTracer(const size_t width, const size_t height) : 
		m_width(width), m_height(height), m_aspectRatio((double)width/height),
		m_data(width * height * 3, 0x00),
	    m_vertical(), m_horizontal(), m_lowerleft(), m_origin() {

		const double viewport_height = 2.0;
		const double viewport_width = m_aspectRatio * viewport_height;
		const double focal_length = 1.0;

		m_horizontal = Vec3(viewport_width, 0.0, 0.0);
		m_vertical = Vec3(0.0, viewport_height, 0.0);
		m_lowerleft = m_origin - m_horizontal / 2 - m_vertical / 2 - Vec3(0.0, 0.0, focal_length);
	}

	const void* GetBitmap() const {
		return m_data.data();
	}

	void Run() {
		static const Vec3 origin = Vec3(0.0, 0.0, 0.0);
		World world;

		constexpr int SAMPLE_COUNT = 4;

		MaterialPtr ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
		MaterialPtr center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
		MaterialPtr left = make_shared<Dielectric>(1.5);
		MaterialPtr right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

		world.AddSphere(Point(0.0, -100.5, -1.0), 100.0, ground);
		world.AddSphere(Point(0.0, 0.0, -1.0), 0.5, center);
		world.AddSphere(Point(-1.0, 0.0, -1.0), -0.5, left);
		world.AddSphere(Point(1.0, 0.0, -1.0), 0.5, right);

		Camera camera(m_width, m_height, 20, Vec3(-2, 2, 1), Vec3(0, 0, -1), Vec3(0, 1, 0));

		constexpr int MAX_REFLECT = 5;
		auto scale = 1.0 / SAMPLE_COUNT;

		for (size_t j = 0; j < m_height; ++j) {

			for (size_t i = 0; i < m_width; ++i) {
				Color pixelColor;

				for (size_t k = 0; k < SAMPLE_COUNT; ++k) {
					double u = ((double)i + random_double()) / (m_width - 1);
					double v = ((double)j + random_double()) / (m_height - 1);

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
			double percent = (double)j / (m_width - 1) * 100;
			std::cout << "Completed: " << std::setprecision(4) << std::setw(7) << percent << "%\r";
		}
	}

private:

	const Color ColorAt(const Ray& ray, Hittable & world, int depth) {
		static constexpr double F_INFINITE = std::numeric_limits<double>::infinity();

		if (depth <= 0) {
			return Color(0, 0, 0); // Absolute black
		}

		HitRecord rec;
		if (world.isHit(ray, rec, 0.000001, F_INFINITE)) {
			Ray scattered;
			Color attenuation;
			if (rec.mat->Scatter(ray, rec, attenuation, scattered)) {
				return attenuation * ColorAt(scattered, world, depth-1);
			}
			return Color(0.0, 0.0, 0.0);
		}
		return SkyColor(ray);
	}

	const Color SkyColor(const Ray & r) const {
		double t = (1.0 + r.direction().unit().y) * 0.5;
		return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7f, 1.0);
	}

private:
	const size_t m_width;
	const size_t m_height;
	const double m_aspectRatio;
	std::vector<byte> m_data;

	// Not const becuase its values are set in the constructor body.
	Vec3 m_origin;
	Vec3 m_vertical;
	Vec3 m_horizontal;
	Vec3 m_lowerleft;
};