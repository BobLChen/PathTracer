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

struct Vector2
{
public:
	float x;
	float y;

	static const Vector2 ZeroVector;
	static const Vector2 UnitVector;
	
public:

	Vector2();

	Vector2(float inX, float inY);

	inline Vector2 operator+(const Vector2& v) const;

	inline Vector2 operator-(const Vector2& v) const;

	inline Vector2 operator*(float scale) const;

	inline Vector2 operator+(float a) const;

	inline Vector2 operator-(float a) const;

	inline Vector2 operator*(const Vector2& v) const;

	inline Vector2 operator/(float scale) const;

	inline Vector2 operator/(const Vector2& v) const;

	inline float operator|(const Vector2& v) const;

	inline float operator^(const Vector2& v) const;

	inline bool operator==(const Vector2& v) const;

	inline bool operator!=(const Vector2& v) const;

	inline bool operator<(const Vector2& other) const;

	inline bool operator>(const Vector2& other) const;

	inline bool operator<=(const Vector2& other) const;

	inline bool operator>=(const Vector2& other) const;

	inline Vector2 operator-() const;

	inline Vector2 operator+=(const Vector2& v);

	inline Vector2 operator-=(const Vector2& v);

	inline Vector2 operator*=(float scale);

	inline Vector2 operator/=(float v);

	inline Vector2 operator*=(const Vector2& v);

	inline Vector2 operator/=(const Vector2& v);

	inline float& operator[](int32 index);

	inline float operator[](int32 index) const;

	inline float& Component(int32 index);

	inline float Component(int32 index) const;

	inline static float DotProduct(const Vector2& a, const Vector2& b);

	inline static float DistSquared(const Vector2& v1, const Vector2& v2);

	inline static float Distance(const Vector2& v1, const Vector2& v2);

	inline static float CrossProduct(const Vector2& a, const Vector2& b);

	inline bool Equals(const Vector2& v, float tolerance = KINDA_SMALL_NUMBER) const;

	inline void Set(float inX, float inY);

	inline float GetMax() const;

	inline float GetAbsMax() const;

	inline float GetMin() const;

	inline float Size() const;

	inline float SizeSquared() const;

	inline Vector2 GetRotated(float angleDeg) const;

	inline Vector2 GetSafeNormal(float tolerance = SMALL_NUMBER) const;

	inline void Normalize(float tolerance = SMALL_NUMBER);

	inline bool IsNearlyZero(float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool IsZero() const;

	inline Vector2 RoundToVector() const;

	inline Vector2 GetAbs() const;

	inline bool ContainsNaN() const
	{
		return (!MMath::IsFinite(x) || !MMath::IsFinite(y));
	}
};

inline Vector2::Vector2()
	: x(0)
	, y(0)
{

}

inline Vector2::Vector2(float inX, float inY)
	: x(inX)
	, y(inY)
{

}

inline Vector2 operator*(float scale, const Vector2& v)
{
	return v.operator*(scale);
}

inline Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

inline Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

inline Vector2 Vector2::operator*(float scale) const
{
	return Vector2(x * scale, y * scale);
}

inline Vector2 Vector2::operator/(float scale) const
{
	const float invScale = 1.f / scale;
	return Vector2(x * invScale, y * invScale);
}

inline Vector2 Vector2::operator+(float a) const
{
	return Vector2(x + a, y + a);
}

inline Vector2 Vector2::operator-(float a) const
{
	return Vector2(x - a, y - a);
}

inline Vector2 Vector2::operator*(const Vector2& v) const
{
	return Vector2(x * v.x, y * v.y);
}

inline Vector2 Vector2::operator/(const Vector2& v) const
{
	return Vector2(x / v.x, y / v.y);
}

inline float Vector2::operator|(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

inline float Vector2::operator^(const Vector2& v) const
{
	return x * v.y - y * v.x;
}

inline float Vector2::DotProduct(const Vector2& a, const Vector2& b)
{
	return a | b;
}

inline float Vector2::DistSquared(const Vector2 &v1, const Vector2 &v2)
{
	return MMath::Square(v2.x - v1.x) + MMath::Square(v2.y - v1.y);
}

inline float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
	return MMath::Sqrt(Vector2::DistSquared(v1, v2));
}

inline float Vector2::CrossProduct(const Vector2& a, const Vector2& b)
{
	return a ^ b;
}

inline bool Vector2::operator==(const Vector2& v) const
{
	return x == v.x && y == v.y;
}

inline bool Vector2::operator!=(const Vector2& v) const
{
	return x != v.x || y != v.y;
}

inline bool Vector2::operator<(const Vector2& other) const
{
	return x < other.x && y < other.y;
}

inline bool Vector2::operator>(const Vector2& other) const
{
	return x > other.x && y > other.y;
}

inline bool Vector2::operator<=(const Vector2& other) const
{
	return x <= other.x && y <= other.y;
}

inline bool Vector2::operator>=(const Vector2& other) const
{
	return x >= other.x && y >= other.y;
}

inline bool Vector2::Equals(const Vector2& v, float tolerance) const
{
	return MMath::Abs(x - v.x) <= tolerance && MMath::Abs(y - v.y) <= tolerance;
}

inline Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+=(const Vector2& v)
{
	x += v.x; 
	y += v.y;
	return *this;
}

inline Vector2 Vector2::operator-=(const Vector2& v)
{
	x -= v.x; 
	y -= v.y;
	return *this;
}

inline Vector2 Vector2::operator*=(float scale)
{
	x *= scale; 
	y *= scale;
	return *this;
}

inline Vector2 Vector2::operator/=(float v)
{
	const float invF = 1.f / v;
	x *= invF; 
	y *= invF;
	return *this;
}

inline Vector2 Vector2::operator*=(const Vector2& v)
{
	x *= v.x; 
	y *= v.y;
	return *this;
}

inline Vector2 Vector2::operator/=(const Vector2& v)
{
	x /= v.x; 
	y /= v.y;
	return *this;
}

inline float& Vector2::operator[](int32 index)
{
	return ((index == 0) ? x : y);
}

inline float Vector2::operator[](int32 index) const
{
	return ((index == 0) ? x : y);
}

inline void Vector2::Set(float inX, float inY)
{
	x = inX;
	y = inY;
}

inline float Vector2::GetMax() const
{
	return MMath::Max(x, y);
}

inline float Vector2::GetAbsMax() const
{
	return MMath::Max(MMath::Abs(x), MMath::Abs(y));
}

inline float Vector2::GetMin() const
{
	return MMath::Min(x, y);
}

inline float Vector2::Size() const
{
	return MMath::Sqrt(x*x + y * y);
}

inline float Vector2::SizeSquared() const
{
	return x * x + y * y;
}

inline Vector2 Vector2::GetRotated(const float angleDeg) const
{
	float s, c;
	MMath::SinCos(&s, &c, MMath::DegreesToRadians(angleDeg));
	return Vector2(c * x - s * y, s * x + c * y);
}

inline Vector2 Vector2::GetSafeNormal(float tolerance) const
{
	const float squareSum = x * x + y * y;
	if (squareSum > tolerance)
	{
		const float scale = MMath::InvSqrt(squareSum);
		return Vector2(x*scale, y*scale);
	}
	return Vector2(0.f, 0.f);
}

inline void Vector2::Normalize(float tolerance)
{
	const float squareSum = x * x + y * y;
	if (squareSum > tolerance)
	{
		const float scale = MMath::InvSqrt(squareSum);
		x *= scale;
		y *= scale;
		return;
	}
	x = 0.0f;
	y = 0.0f;
}

inline bool Vector2::IsNearlyZero(float tolerance) const
{
	return MMath::Abs(x) <= tolerance && MMath::Abs(y) <= tolerance;
}

inline bool Vector2::IsZero() const
{
	return x == 0.f && y == 0.f;
}

inline float& Vector2::Component(int32 index)
{
	return (&x)[index];
}

inline float Vector2::Component(int32 index) const
{
	return (&x)[index];
}

inline Vector2 Vector2::RoundToVector() const
{
	return Vector2((float)MMath::RoundToInt(x), (float)MMath::RoundToInt(y));
}

inline Vector2 Vector2::GetAbs() const
{
	return Vector2(MMath::Abs(x), MMath::Abs(y));
}