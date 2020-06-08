/**********************************************************************
Copyright (c) 2020 BobLChen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************/

#pragma once

#include <string>

#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"

struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

public:
    
	Vector4(const Vector3& inVector, float inW = 1.0f);

	explicit Vector4(float inX = 0.0f, float inY = 0.0f, float inZ = 0.0f, float inW = 1.0f);

	explicit Vector4(Vector2 inXY, Vector2 inZW);

	inline float& operator[](int32 index);

	inline float operator[](int32 index) const;

	inline Vector4 operator-() const;

	inline Vector4 operator+(const Vector4& v) const;

	inline Vector4 operator+=(const Vector4& v);

	inline Vector4 operator-(const Vector4& v) const;

	inline Vector4 operator-=(const Vector4& v);

	inline Vector4 operator*(float scale) const;

	inline Vector4 operator/(float scale) const;

	inline Vector4 operator/(const Vector4& v) const;

	inline Vector4 operator*(const Vector4& v) const;

	inline Vector4 operator*=(const Vector4& v);

	inline Vector4 operator/=(const Vector4& v);

	inline Vector4 operator*=(float f);

	inline bool operator==(const Vector4& v) const;

	inline bool operator!=(const Vector4& v) const;

	inline Vector4 operator^(const Vector4& v) const;

	inline float& Component(int32 index);

	inline const float& Component(int32 index) const;

	inline bool Equals(const Vector4& v, float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool IsUnit3(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const;

	inline Vector4 GetSafeNormal(float tolerance = SMALL_NUMBER) const;

	inline Vector4 GetUnsafeNormal3() const;

	inline void Set(float inX, float inY, float inZ, float inW);

	inline float Size3() const;

	inline float SizeSquared3() const;

	inline float Size() const;

	inline float SizeSquared() const;

	inline bool ContainsNaN() const;

	inline bool IsNearlyZero3(float tolerance = KINDA_SMALL_NUMBER) const;

	inline Vector4 Reflect3(const Vector4& normal) const;

	inline void FindBestAxisVectors3(Vector4& axis1, Vector4& axis2) const;

	inline static Vector4 Min(const Vector4& a, const Vector4& b);

	inline static Vector4 Max(const Vector4& a, const Vector4& b);

	friend inline float Dot3(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	friend inline float Dot4(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	friend inline Vector4 operator*(float scale, const Vector4& v)
	{
		return v.operator*(scale);
	}
};

inline Vector4::Vector4(const Vector3& inVector, float inW)
	: x(inVector.x)
	, y(inVector.y)
	, z(inVector.z)
	, w(inW)
{
	
}

inline Vector4::Vector4(float inX, float inY, float inZ, float inW)
	: x(inX)
	, y(inY)
	, z(inZ)
	, w(inW)
{
	
}

inline Vector4::Vector4(Vector2 inXY, Vector2 inZW)
	: x(inXY.x)
	, y(inXY.y)
	, z(inZW.x)
	, w(inZW.y)
{
	
}

inline float& Vector4::operator[](int32 index)
{
	return (&x)[index];
}

inline float Vector4::operator[](int32 index) const
{
	return (&x)[index];
}

inline void Vector4::Set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

inline Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

inline Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vector4 Vector4::operator+=(const Vector4& v)
{
	x += v.x; 
	y += v.y; 
	z += v.z; 
	w += v.w;
	
	return *this;
}

inline Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline Vector4 Vector4::operator-=(const Vector4& v)
{
	x -= v.x; 
	y -= v.y; 
	z -= v.z; 
	w -= v.w;
	
	return *this;
}

inline Vector4 Vector4::operator*(float scale) const
{
	return Vector4(x * scale, y * scale, z * scale, w * scale);
}

inline Vector4 Vector4::operator/(float scale) const
{
	const float invScale = 1.f / scale;
	return Vector4(x * invScale, y * invScale, z * invScale, w * invScale);
}

inline Vector4 Vector4::operator*(const Vector4& v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline Vector4 Vector4::operator^(const Vector4& v) const
{
	return Vector4(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x,
		0.0f
	);
}

inline float& Vector4::Component(int32 index)
{
	return (&x)[index];
}

inline const float& Vector4::Component(int32 index) const
{
	return (&x)[index];
}

inline bool Vector4::operator==(const Vector4& v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

inline bool Vector4::operator!=(const Vector4& v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
}

inline bool Vector4::Equals(const Vector4& v, float tolerance) const
{
	return MMath::Abs(x - v.x) <= tolerance && MMath::Abs(y - v.y) <= tolerance && MMath::Abs(z - v.z) <= tolerance && MMath::Abs(w - v.w) <= tolerance;
}

inline Vector4 Vector4::GetSafeNormal(float tolerance) const
{
	const float squareSum = x * x + y * y + z * z;
	if (squareSum > tolerance)
	{
		const float scale = MMath::InvSqrt(squareSum);
		return Vector4(x * scale, y * scale, z * scale, 0.0f);
	}
	return Vector4(0.f);
}

inline Vector4 Vector4::GetUnsafeNormal3() const
{
	const float scale = MMath::InvSqrt(x * x + y * y + z * z);
	return Vector4(x * scale, y * scale, z * scale, 0.0f);
}

inline float Vector4::Size3() const
{
	return MMath::Sqrt(x * x + y * y + z * z);
}

inline float Vector4::SizeSquared3() const
{
	return x * x + y * y + z * z;
}

inline float Vector4::Size() const
{
	return MMath::Sqrt(x * x + y * y + z * z + w * w);
}

inline float Vector4::SizeSquared() const
{
	return x * x + y * y + z * z + w * w;
}

inline bool Vector4::IsUnit3(float LengthSquaredTolerance) const
{
	return MMath::Abs(1.0f - SizeSquared3()) < LengthSquaredTolerance;
}

inline bool Vector4::ContainsNaN() const
{
	return (
		!MMath::IsFinite(x) ||
		!MMath::IsFinite(y) ||
		!MMath::IsFinite(z) ||
		!MMath::IsFinite(w)
	);
}

inline bool Vector4::IsNearlyZero3(float tolerance) const
{
	return
		MMath::Abs(x) <= tolerance && 
		MMath::Abs(y) <= tolerance && 
		MMath::Abs(z) <= tolerance;
}

inline Vector4 Vector4::Reflect3(const Vector4& normal) const
{
	return 2.0f * Dot3(*this, normal) * normal - *this;
}

inline Vector4 Vector4::Min(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	result.x = MMath::Min(a.x, b.x);
	result.y = MMath::Min(a.y, b.y);
	result.z = MMath::Min(a.z, b.z);
	result.w = MMath::Min(a.w, b.w);
	return result;
}

inline Vector4 Vector4::Max(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	result.x = MMath::Max(a.x, b.x);
	result.y = MMath::Max(a.y, b.y);
	result.z = MMath::Max(a.z, b.z);
	result.w = MMath::Max(a.w, b.w);
	return result;
}

inline void Vector4::FindBestAxisVectors3(Vector4& axis1, Vector4& axis2) const
{
	const float nx = MMath::Abs(x);
	const float ny = MMath::Abs(y);
	const float nz = MMath::Abs(z);

	if (nz > nx && nz > ny)
	{
		axis1 = Vector4(1, 0, 0);
	}
	else 
	{
		axis1 = Vector4(0, 0, 1);
	}

	axis1 = (axis1 - *this * Dot3(axis1, *this)).GetSafeNormal();
	axis2 = axis1 ^ *this;
}

inline Vector4 Vector4::operator*=(const Vector4& v)
{
	x *= v.x; 
	y *= v.y; 
	z *= v.z; 
	w *= v.w;
	
	return *this;
}

inline Vector4 Vector4::operator/=(const Vector4& v)
{
	x /= v.x; 
	y /= v.y; 
	z /= v.z;
	w /= v.w;
	
	return *this;
}

inline Vector4 Vector4::operator*=(float f)
{
	x *= f; 
	y *= f; 
	z *= f; 
	w *= f;
	
	return *this;
}

inline Vector4 Vector4::operator/(const Vector4& v) const
{
	return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}