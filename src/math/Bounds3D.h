#pragma once

#include "math/Math.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Bounds3D
{
public:
	glm::vec3 min;
	glm::vec3 max;

public:
	Bounds3D()
		: min(+INT16_MAX, +INT16_MAX, +INT16_MAX)
		, max(-INT16_MAX, -INT16_MAX, -INT16_MAX)
	{

	}

	Bounds3D(const glm::vec3& p)
		: min(p)
		, max(p)
	{

	}

	Bounds3D(const glm::vec3& p1, const glm::vec3& p2)
		: min(glm::min(p1, p2))
		, max(glm::max(p1, p2))
	{

	}

	glm::vec3 Center() const
	{
		return (min + max) * 0.5f;
	}

	glm::vec3 Extents() const
	{
		return max - min;
	}

	bool Contains(const glm::vec3& p) const
	{
		glm::vec3 radius = 0.5f * Extents();
		glm::vec3 center = Center();

		return 
			std::abs(center.x - p.x) <= radius.x &&
			std::abs(center.y - p.y) <= radius.y &&
			std::abs(center.z - p.z) <= radius.z;
	}

	int Maxdim() const
	{
		glm::vec3 ext = Extents();

		if (ext.x >= ext.y && ext.x >= ext.z) {
			return 0;
		}
			
		if (ext.y >= ext.x && ext.y >= ext.z) {
			return 1;
		}
			
		if (ext.z >= ext.x && ext.z >= ext.y) {
			return 2;
		}
		
		return 0;
	}

	float Area() const
	{
		glm::vec3 ext = Extents();
		return 2.f * (ext.x * ext.y + ext.x * ext.z + ext.y * ext.z);
	}

	const glm::vec3& operator [] (int i) const 
	{ 
		if (i == 0) {
			return min;
		}
		else 
		{
			return max;
		}
	}

	void Expand(const glm::vec3& p)
	{
		min = glm::min(min, p);
		max = glm::max(max, p);
	}

	void Expand(const Bounds3D& b)
	{
		min = glm::min(min, b.min);
		max = glm::max(max, b.max);
	}

	static Bounds3D Union(const Bounds3D& box1, const Bounds3D& box2)
	{
		return Bounds3D(glm::min(box1.min, box2.min), glm::max(box1.max, box2.max));
	}

	static Bounds3D Intersection(const Bounds3D& box1, const Bounds3D& box2)
	{
		return Bounds3D(glm::max(box1.min, box2.min), glm::min(box1.max, box2.max));
	}

	static void Intersection(const Bounds3D& box1, const Bounds3D& box2, Bounds3D& box)
	{
		box.min = glm::max(box1.min, box2.min);
		box.max = glm::min(box1.max, box2.max);
	}

	static bool Contains(const Bounds3D& box1, const Bounds3D& box2)
	{
		return box1.Contains(box2.min) && box1.Contains(box2.max);
	}
};
