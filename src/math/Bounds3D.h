#pragma once

#include "math/Math.h"
#include "math/Vector3.h"

class Bounds3D
{
public:
	Vector3 min;
	Vector3 max;

public:
	Bounds3D()
		: min(+INT16_MAX, +INT16_MAX, +INT16_MAX)
		, max(-INT16_MAX, -INT16_MAX, -INT16_MAX)
	{

	}

	Bounds3D(const Vector3& p)
		: min(p)
		, max(p)
	{

	}

	Bounds3D(const Vector3& inMin, const Vector3& inMax)
        : min(Vector3::Min(inMin, inMax))
        , max(Vector3::Max(inMin, inMax))
	{
        
	}

	Vector3 Center() const
	{
		return (min + max) * 0.5f;
	}

	Vector3 Extents() const
	{
		return max - min;
	}

	bool Contains(const Vector3& p) const
	{
		Vector3 radius = 0.5f * Extents();
		Vector3 center = Center();
        
		return 
			MMath::Abs(center.x - p.x) <= radius.x &&
			MMath::Abs(center.y - p.y) <= radius.y &&
			MMath::Abs(center.z - p.z) <= radius.z;
	}

	int Maxdim() const
	{
		Vector3 ext = Extents();

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
		Vector3 ext = Extents();
		return 2.f * (ext.x * ext.y + ext.x * ext.z + ext.y * ext.z);
	}

	const Vector3& operator [] (int i) const
	{ 
		if (i == 0) {
			return min;
		}
		else 
		{
			return max;
		}
	}

	void Expand(const Vector3& p)
	{
        min = Vector3::Min(min, p);
        max = Vector3::Max(max, p);
	}

	void Expand(const Bounds3D& b)
	{
        min = Vector3::Min(min, b.min);
        max = Vector3::Max(max, b.max);
	}

	static Bounds3D Union(const Bounds3D& box1, const Bounds3D& box2)
	{
        return Bounds3D(Vector3::Min(box1.min, box2.min), Vector3::Max(box1.max, box2.max));
	}

	static Bounds3D Intersection(const Bounds3D& box1, const Bounds3D& box2)
	{
        return Bounds3D(Vector3::Max(box1.min, box2.min), Vector3::Min(box1.max, box2.max));
	}

	static void Intersection(const Bounds3D& box1, const Bounds3D& box2, Bounds3D& box)
	{
        box.min = Vector3::Max(box1.min, box2.min);
        box.max = Vector3::Min(box1.max, box2.max);
	}

	static bool Contains(const Bounds3D& box1, const Bounds3D& box2)
	{
		return box1.Contains(box2.min) && box1.Contains(box2.max);
	}
};
