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

#include "Math.h"
#include "Vector2.h"

struct Vector4;

struct Vector3
{
public:
	float x;
	float y;
	float z;

	static const Vector3 ZeroVector;
	static const Vector3 OneVector;
	static const Vector3 UpVector;
	static const Vector3 ForwardVector;
	static const Vector3 RightVector;

public:
	
	Vector3(const Vector4& v);

	Vector3(float inX, float inY, float inZ);

	explicit Vector3(float inF);

	explicit Vector3(const Vector2 v, float inZ);

	explicit Vector3();

	inline void Scale(float scale);

	inline Vector3 operator^(const Vector3& v) const;

	inline static Vector3 CrossProduct(const Vector3& a, const Vector3& b);

	inline float operator|(const Vector3& v) const;

	inline static float DotProduct(const Vector3& a, const Vector3& b);
	
	inline Vector3 operator+(const Vector3& v) const;

	inline Vector3 operator-(const Vector3& v) const;

	inline Vector3 operator-(float bias) const;

	inline Vector3 operator+(float bias) const;

	inline Vector3 operator*(float scale) const;

	inline Vector3 operator/(float scale) const;

	inline Vector3 operator*(const Vector3& v) const;

	inline Vector3 operator/(const Vector3& v) const;

	inline bool operator==(const Vector3& v) const;

	inline bool operator!=(const Vector3& v) const;

	inline bool Equals(const Vector3& v, float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool AllComponentsEqual(float tolerance = KINDA_SMALL_NUMBER) const;

	inline Vector3 operator-() const;

	inline Vector3 operator+=(const Vector3& v);

	inline Vector3 operator-=(const Vector3& v);

	inline Vector3 operator*=(float scale);

	inline Vector3 operator/=(float v);

	inline Vector3 operator*=(const Vector3& v);

	inline Vector3 operator/=(const Vector3& v);

	inline float& operator[](int32 index);

	inline float operator[](int32 index)const;

	inline float& component(int32 index);

	inline float component(int32 index) const;

	inline void Set(float inX, float inY, float inZ);

	inline float GetMax() const;

	inline float GetAbsMax() const;

	inline float GetMin() const;

	inline float GetAbsMin() const;

	inline Vector3 ComponentMin(const Vector3& other) const;

	inline Vector3 ComponentMax(const Vector3& other) const;

	inline Vector3 GetAbs() const;

	inline static Vector3 Min(const Vector3& a, const Vector3& b);

	inline static Vector3 Max(const Vector3& a, const Vector3& b);
    
    inline static Vector3 Clamp(const Vector3& val, const Vector3& mmin, const Vector3& mmax);

	inline float Size() const;

	inline float SizeSquared() const;

	inline float Size2D() const;

	inline float SizeSquared2D() const;

	inline bool IsNearlyZero(float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool IsZero() const;

	inline bool Normalize(float tolerance = SMALL_NUMBER);

	inline bool IsNormalized() const;

	inline Vector3 GetUnsafeNormal() const;

	inline bool IsUniform(float tolerance = KINDA_SMALL_NUMBER) const;

	inline Vector3 GetSafeNormal(float tolerance = SMALL_NUMBER) const;

	inline Vector3 GetSafeNormal2D(float tolerance = SMALL_NUMBER) const;

	inline Vector3 GetUnsafeNormal2D() const;

	inline bool ContainsNaN() const;

	static inline float Dist(const Vector3 &v1, const Vector3 &v2);

	static inline float Distance(const Vector3 &v1, const Vector3 &v2) { return Dist(v1, v2); }

	static inline float DistXY(const Vector3 &v1, const Vector3 &v2);

	static inline float Dist2D(const Vector3 &v1, const Vector3 &v2) { return DistXY(v1, v2); }

	static inline float DistSquared(const Vector3 &v1, const Vector3 &v2);

	static inline float DistSquaredXY(const Vector3 &v1, const Vector3 &v2);

	static inline float DistSquared2D(const Vector3 &v1, const Vector3 &v2) { return DistSquaredXY(v1, v2); }

	static inline Vector3 RadiansToDegrees(const Vector3& radVector);

	static inline Vector3 DegreesToRadians(const Vector3& degVector);
};

inline Vector3::Vector3(const Vector2 v, float inZ)
	: x(v.x), y(v.y), z(inZ)
{

}

inline Vector3::Vector3()
{

}

inline Vector3::Vector3(float inF)
	: x(inF)
	, y(inF)
	, z(inF)
{

}

inline Vector3::Vector3(float inX, float inY, float inZ)
	: x(inX)
	, y(inY)
	, z(inZ)
{
	
}

inline Vector3 operator*(float scale, const Vector3& v)
{
	return v.operator*(scale);
}

inline Vector3 Vector3::Clamp(const Vector3& val, const Vector3& mmin, const Vector3& mmax)
{
    return Vector3(
        MMath::Clamp(val.x, mmin.x, mmax.x),
        MMath::Clamp(val.y, mmin.y, mmax.y),
        MMath::Clamp(val.z, mmin.z, mmax.z)
    );
}

inline Vector3 Vector3::Min(const Vector3& a, const Vector3& b)
{
	Vector3 result;
	result.x = MMath::Min(a.x, b.x);
	result.y = MMath::Min(a.y, b.y);
	result.z = MMath::Min(a.z, b.z);
	return result;
}

inline Vector3 Vector3::Max(const Vector3& a, const Vector3& b)
{
	Vector3 result;
	result.x = MMath::Max(a.x, b.x);
	result.y = MMath::Max(a.y, b.y);
	result.z = MMath::Max(a.z, b.z);
	return result;
}

inline Vector3 Vector3::RadiansToDegrees(const Vector3& radVector)
{
	return radVector * (180.f / PI);
}

inline Vector3 Vector3::DegreesToRadians(const Vector3& degVector)
{
	return degVector * (PI / 180.f);
}

inline Vector3 Vector3::operator^(const Vector3& v) const
{
	return Vector3
	(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

inline Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& b)
{
	return a ^ b;
}

inline float Vector3::operator|(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline float Vector3::DotProduct(const Vector3& a, const Vector3& b)
{
	return a | b;
}

inline Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3 Vector3::operator-(float bias) const
{
	return Vector3(x - bias, y - bias, z - bias);
}

inline Vector3 Vector3::operator+(float bias) const
{
	return Vector3(x + bias, y + bias, z + bias);
}

inline Vector3 Vector3::operator*(float scale) const
{
	return Vector3(x * scale, y * scale, z * scale);
}

inline void Vector3::Scale(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

inline Vector3 Vector3::operator/(float scale) const
{
	const float RScale = 1.f / scale;
	return Vector3(x * RScale, y * RScale, z * RScale);
}

inline Vector3 Vector3::operator*(const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

inline Vector3 Vector3::operator/(const Vector3& v) const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

inline bool Vector3::operator==(const Vector3& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

inline bool Vector3::operator!=(const Vector3& v) const
{
	return x != v.x || y != v.y || z != v.z;
}

inline bool Vector3::Equals(const Vector3& v, float tolerance) const
{
	return MMath::Abs(x - v.x) <= tolerance && MMath::Abs(y - v.y) <= tolerance && MMath::Abs(z - v.z) <= tolerance;
}

inline bool Vector3::AllComponentsEqual(float tolerance) const
{
	return MMath::Abs(x - y) <= tolerance && MMath::Abs(x - z) <= tolerance && MMath::Abs(y - z) <= tolerance;
}

inline Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator+=(const Vector3& v)
{
	x += v.x; 
	y += v.y; 
	z += v.z;
	
	return *this;
}

inline Vector3 Vector3::operator-=(const Vector3& v)
{
	x -= v.x; 
	y -= v.y; 
	z -= v.z;
	
	return *this;
}

inline Vector3 Vector3::operator*=(float scale)
{
	x *= scale; 
	y *= scale; 
	z *= scale;
	
	return *this;
}

inline Vector3 Vector3::operator/=(float v)
{
	const float RV = 1.f / v;
	x *= RV; 
	y *= RV; 
	z *= RV;
	
	return *this;
}

inline Vector3 Vector3::operator*=(const Vector3& v)
{
	x *= v.x; 
	y *= v.y; 
	z *= v.z;
	
	return *this;
}

inline Vector3 Vector3::operator/=(const Vector3& v)
{
	x /= v.x; 
	y /= v.y; 
	z /= v.z;
	
	return *this;
}

inline float& Vector3::operator[](int32 index)
{
	if (index == 0) {
		return x;
	}
	else if (index == 1) {
		return y;
	}
	else {
		return z;
	}
}

inline float Vector3::operator[](int32 index)const
{
	if (index == 0) {
		return x;
	}
	else if (index == 1) {
		return y;
	}
	else {
		return z;
	}
}

inline void Vector3::Set(float inX, float inY, float inZ)
{
	x = inX;
	y = inY;
	z = inZ;
	
}

inline float Vector3::GetMax() const
{
	return MMath::Max(MMath::Max(x, y), z);
}

inline float Vector3::GetAbsMax() const
{
	return MMath::Max(MMath::Max(MMath::Abs(x), MMath::Abs(y)), MMath::Abs(z));
}

inline float Vector3::GetMin() const
{
	return MMath::Min(MMath::Min(x, y), z);
}

inline float Vector3::GetAbsMin() const
{
	return MMath::Min(MMath::Min(MMath::Abs(x), MMath::Abs(y)), MMath::Abs(z));
}

inline Vector3 Vector3::ComponentMin(const Vector3& other) const
{
	return Vector3(MMath::Min(x, other.x), MMath::Min(y, other.y), MMath::Min(z, other.z));
}

inline Vector3 Vector3::ComponentMax(const Vector3& other) const
{
	return Vector3(MMath::Max(x, other.x), MMath::Max(y, other.y), MMath::Max(z, other.z));
}

inline Vector3 Vector3::GetAbs() const
{
	return Vector3(MMath::Abs(x), MMath::Abs(y), MMath::Abs(z));
}

inline float Vector3::Size() const
{
	return MMath::Sqrt(x * x + y * y + z * z);
}

inline float Vector3::SizeSquared() const
{
	return x * x + y * y + z * z;
}

inline float Vector3::Size2D() const
{
	return MMath::Sqrt(x*x + y * y);
}

inline float Vector3::SizeSquared2D() const
{
	return x * x + y * y;
}

inline bool Vector3::IsNearlyZero(float tolerance) const
{
	return
		MMath::Abs(x) <= tolerance && 
		MMath::Abs(y) <= tolerance && 
		MMath::Abs(z) <= tolerance;
}

inline bool Vector3::IsZero() const
{
	return x == 0.f && y == 0.f && z == 0.f;
}

inline bool Vector3::Normalize(float tolerance)
{
	const float squareSum = x * x + y * y + z * z;
	if (squareSum > tolerance)
	{
		const float scale = MMath::InvSqrt(squareSum);
		x *= scale; 
		y *= scale; 
		z *= scale;
		return true;
	}
	return false;
}

inline bool Vector3::IsNormalized() const
{
	return (MMath::Abs(1.f - SizeSquared()) < THRESH_VECTOR_NORMALIZED);
}

inline Vector3 Vector3::GetUnsafeNormal() const
{
	const float scale = MMath::InvSqrt(x*x + y * y + z * z);
	return Vector3(x * scale, y * scale, z * scale);
}

inline float& Vector3::component(int32 index)
{
	return (&x)[index];
}

inline float Vector3::component(int32 index) const
{
	return (&x)[index];
}

inline bool Vector3::IsUniform(float tolerance) const
{
	return AllComponentsEqual(tolerance);
}

inline Vector3 Vector3::GetSafeNormal(float tolerance) const
{
	const float squareSum = x * x + y * y + z * z;
	if (squareSum == 1.f) {
		return *this;
	}
	else if (squareSum < tolerance) {
		return Vector3::ZeroVector;
	}
	const float scale = MMath::InvSqrt(squareSum);
	return Vector3(x * scale, y * scale, z * scale);
}

inline Vector3 Vector3::GetSafeNormal2D(float tolerance) const
{
	const float squareSum = x * x + y * y;
	if (squareSum == 1.f) 
	{
		if (z == 0.f) {
			return *this;
		}
		else {
			return Vector3(x, y, 0.f);
		}
	}
	else if (squareSum < tolerance) 
	{
		return Vector3::ZeroVector;
	}
	const float scale = MMath::InvSqrt(squareSum);
	return Vector3(x*scale, y*scale, 0.f);
}

inline Vector3 Vector3::GetUnsafeNormal2D() const
{
	const float scale = MMath::InvSqrt(x * x + y * y);
	return Vector3(x * scale, y * scale, 0.f);
}

inline bool Vector3::ContainsNaN() const
{
	return (
		!MMath::IsFinite(x) || 
		!MMath::IsFinite(y) ||
		!MMath::IsFinite(z)
	);
}

inline float Vector3::Dist(const Vector3 &v1, const Vector3 &v2)
{
	return MMath::Sqrt(Vector3::DistSquared(v1, v2));
}

inline float Vector3::DistXY(const Vector3 &v1, const Vector3 &v2)
{
	return MMath::Sqrt(Vector3::DistSquaredXY(v1, v2));
}

inline float Vector3::DistSquared(const Vector3 &v1, const Vector3 &v2)
{
	return MMath::Square(v2.x - v1.x) + MMath::Square(v2.y - v1.y) + MMath::Square(v2.z - v1.z);
}

inline float Vector3::DistSquaredXY(const Vector3 &v1, const Vector3 &v2)
{
	return MMath::Square(v2.x - v1.x) + MMath::Square(v2.y - v1.y);
}

inline Vector3 ClampVector(const Vector3& v, const Vector3& min, const Vector3& max)
{
	return Vector3(
		MMath::Clamp(v.x, min.x, max.x),
		MMath::Clamp(v.y, min.y, max.y),
		MMath::Clamp(v.z, min.z, max.z)
	);
}
