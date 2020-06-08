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
#include "Axis.h"
#include "Vector3.h"
#include "Matrix4x4.h"

struct Quat
{
public:
	float x;
	float y;
	float z;
	float w;

	static const Quat Identity;

public:

	Quat();

	Quat(float inX, float inY, float inZ, float inW);

	Quat(const Quat& q);

	Quat(Vector3 axis, float angleRad);

	explicit Quat(const Matrix4x4& m);

	inline Quat operator+(const Quat& q) const;

	inline Quat operator+=(const Quat& q);

	inline Quat operator-(const Quat& q) const;

	inline bool Equals(const Quat& q, float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool IsIdentity(float tolerance = SMALL_NUMBER) const;

	inline Quat operator-=(const Quat& q);

	inline Quat operator*(const Quat& q) const;

	inline Quat operator*=(const Quat& q);

	inline Vector3 operator*(const Vector3& v) const;

	inline Matrix4x4 operator*(const Matrix4x4& m) const;

	inline Quat operator*=(const float scale);

	inline Quat operator*(const float scale) const;

	inline Quat operator/=(const float scale);

	inline Quat operator/(const float scale) const;

	inline bool operator==(const Quat& q) const;

	inline bool operator!=(const Quat& q) const;

	inline float operator|(const Quat& q) const;

	inline void Normalize(float tolerance = SMALL_NUMBER);

	inline Quat GetNormalized(float tolerance = SMALL_NUMBER) const;

	inline bool IsNormalized() const;

	inline float Size() const;

	inline float SizeSquared() const;

	inline float GetAngle() const;

	inline void ToAxisAndAngle(Vector3& axis, float& angle) const;

	inline Vector3 RotateVector(Vector3 v) const;

	inline Vector3 UnrotateVector(Vector3 v) const;

	inline Quat Log() const;

	inline Quat Exp() const;

	inline Quat Inverse() const;

	inline Vector3 GetAxisX() const;

	inline Vector3 GetAxisY() const;

	inline Vector3 GetAxisZ() const;

	inline Vector3 GetForwardVector() const;

	inline Vector3 GetRightVector() const;

	inline Vector3 GetUpVector() const;

	inline Vector3 GetVector() const;

	inline Vector3 GetRotationAxis() const;

	inline Matrix4x4 ToMatrix() const;

	inline void ToMatrix(Matrix4x4& outMatrix) const;

	inline bool ContainsNaN() const;
};

inline Quat::Quat(const Matrix4x4& m)
{
	if (m.GetScaledAxis(Axis::X).IsNearlyZero() || m.GetScaledAxis(Axis::Y).IsNearlyZero() || m.GetScaledAxis(Axis::Z).IsNearlyZero())
	{
		*this = Quat::Identity;
		return;
	}

	float s;

	const float tr = m.m[0][0] + m.m[1][1] + m.m[2][2];

	if (tr > 0.0f)
	{
		float invS = MMath::InvSqrt(tr + 1.f);
		this->w = 0.5f * (1.f / invS);
		s = 0.5f * invS;

		this->x = (m.m[1][2] - m.m[2][1]) * s;
		this->y = (m.m[2][0] - m.m[0][2]) * s;
		this->z = (m.m[0][1] - m.m[1][0]) * s;
	}
	else
	{
		int32 i = 0;

		if (m.m[1][1] > m.m[0][0]) {
			i = 1;
		}

		if (m.m[2][2] > m.m[i][i]) {
			i = 2;
		}
		
		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;

		float invS = MMath::InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / invS);

		s = 0.5f * invS;

		qt[3] = (m.m[j][k] - m.m[k][j]) * s;
		qt[j] = (m.m[i][j] + m.m[j][i]) * s;
		qt[k] = (m.m[i][k] + m.m[k][i]) * s;

		this->x = qt[0];
		this->y = qt[1];
		this->z = qt[2];
		this->w = qt[3];
	}
}

inline Quat::Quat()
{

}

inline Quat::Quat(float inX, float inY, float inZ, float inW)
	: x(inX)
	, y(inY)
	, z(inZ)
	, w(inW)
{

}

inline Quat::Quat(const Quat& q)
	: x(q.x)
	, y(q.y)
	, z(q.z)
	, w(q.w)
{

}

inline Quat::Quat(Vector3 axis, float angleRad)
{
	const float halfA = 0.5f * angleRad;
	float s, c;
	MMath::SinCos(&s, &c, halfA);

	x = s * axis.x;
	y = s * axis.y;
	z = s * axis.z;
	w = c;
}

inline Vector3 Quat::operator*(const Vector3& v) const
{
	return RotateVector(v);
}

inline Matrix4x4 Quat::operator*(const Matrix4x4& m) const
{
	Matrix4x4 result;
	Quat vt, vr;
	Quat inv = Inverse();
	for (int32 I = 0; I < 4; ++I)
	{
		Quat VQ(m.m[I][0], m.m[I][1], m.m[I][2], m.m[I][3]);
		MMath::VectorQuaternionMultiply(&vt, this, &VQ);
		MMath::VectorQuaternionMultiply(&vr, &vt, &inv);
		result.m[I][0] = vr.x;
		result.m[I][1] = vr.y;
		result.m[I][2] = vr.z;
		result.m[I][3] = vr.w;
	}
	return result;
}

inline Quat Quat::operator+(const Quat& q) const
{
	return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quat Quat::operator+=(const Quat& q)
{
	this->x += q.x;
	this->y += q.y;
	this->z += q.z;
	this->w += q.w;
	
	return *this;
}

inline Quat Quat::operator-(const Quat& q) const
{
	return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline bool Quat::Equals(const Quat& q, float tolerance) const
{
	return (MMath::Abs(x - q.x) <= tolerance && MMath::Abs(y - q.y) <= tolerance && MMath::Abs(z - q.z) <= tolerance && MMath::Abs(w - q.w) <= tolerance)
		|| (MMath::Abs(x + q.x) <= tolerance && MMath::Abs(y + q.y) <= tolerance && MMath::Abs(z + q.z) <= tolerance && MMath::Abs(w + q.w) <= tolerance);
}

inline bool Quat::IsIdentity(float tolerance) const
{
	return Equals(Quat::Identity, tolerance);
}

inline Quat Quat::operator-=(const Quat& q)
{
	this->x -= q.x;
	this->y -= q.y;
	this->z -= q.z;
	this->w -= q.w;
	
	return *this;
}

inline Quat Quat::operator*=(const Quat& q)
{
	MMath::VectorQuaternionMultiply(this, this, &q);
    return *this;
}

inline Quat Quat::operator*(const Quat& q) const
{
	Quat result;
	MMath::VectorQuaternionMultiply((void*)(&result), (const void*)(this), (const void*)(&q));
	return result;
}

inline Quat Quat::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
	
	return *this;
}

inline Quat Quat::operator*(const float scale) const
{
	return Quat(scale * x, scale * y, scale * z, scale * w);
}

inline Quat Quat::operator/=(const float scale)
{
	const float recip = 1.0f / scale;
	x *= recip;
	y *= recip;
	z *= recip;
	w *= recip;
	
	return *this;
}

inline Quat Quat::operator/(const float scale) const
{
	const float recip = 1.0f / scale;
	return Quat(x * recip, y * recip, z * recip, w * recip);
}

inline bool Quat::operator==(const Quat& q) const
{
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

inline bool Quat::operator!=(const Quat& q) const
{
	return x != q.x || y != q.y || z != q.z || w != q.w;
}

inline float Quat::operator|(const Quat& q) const
{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}

inline void Quat::Normalize(float tolerance)
{
	const float squareSum = x * x + y * y + z * z + w * w;
	if (squareSum >= tolerance)
	{
		const float scale = MMath::InvSqrt(squareSum);
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
	}
	else
	{
		*this = Quat::Identity;
	}
}

inline Quat Quat::GetNormalized(float tolerance) const
{
	Quat result(*this);
	result.Normalize(tolerance);
	return result;
}

inline bool Quat::IsNormalized() const
{
	return (MMath::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
}

inline float Quat::Size() const
{
	return MMath::Sqrt(x * x + y * y + z * z + w * w);
}

inline float Quat::SizeSquared() const
{
	return (x * x + y * y + z * z + w * w);
}

inline float Quat::GetAngle() const
{
	return 2.f * MMath::Acos(w);
}

inline void Quat::ToAxisAndAngle(Vector3& axis, float& angle) const
{
	angle = GetAngle();
	axis = GetRotationAxis();
}

inline Vector3 Quat::GetRotationAxis() const
{
	const float s = MMath::Sqrt(MMath::Max(1.f - (w * w), 0.f));
	if (s >= 0.0001f) {
		return Vector3(x / s, y / s, z / s);
	}
	return Vector3(1.f, 0.f, 0.f);
}

inline Vector3 Quat::RotateVector(Vector3 v) const
{
	const Vector3 q(x, y, z);
	const Vector3 t = 2.f * Vector3::CrossProduct(q, v);
	const Vector3 result = v + (w * t) + Vector3::CrossProduct(q, t);
	return result;
}

inline Vector3 Quat::UnrotateVector(Vector3 v) const
{
	const Vector3 q(-x, -y, -z);
	const Vector3 t = 2.f * Vector3::CrossProduct(q, v);
	const Vector3 result = v + (w * t) + Vector3::CrossProduct(q, t);
	return result;
}

inline Quat Quat::Inverse() const
{
	return Quat(-x, -y, -z, w);
}

inline Quat Quat::Log() const
{
	Quat result;
	result.w = 0.f;

	if (MMath::Abs(w) < 1.f)
	{
		const float angle = MMath::Acos(w);
		const float sinAngle = MMath::Sin(angle);

		if (MMath::Abs(sinAngle) >= SMALL_NUMBER)
		{
			const float scale = angle / sinAngle;
			result.x = scale * x;
			result.y = scale * y;
			result.z = scale * z;

			return result;
		}
	}

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}

inline Quat Quat::Exp() const
{
	const float angle = MMath::Sqrt(x * x + y * y + z * z);
	const float sinAngle = MMath::Sin(angle);

	Quat result;
	result.w = MMath::Cos(angle);

	if (MMath::Abs(sinAngle) >= SMALL_NUMBER)
	{
		const float scale = sinAngle / angle;
		result.x = scale * x;
		result.y = scale * y;
		result.z = scale * z;
	}
	else
	{
		result.x = x;
		result.y = y;
		result.z = z;
	}

	return result;
}

inline Vector3 Quat::GetAxisX() const
{
	return RotateVector(Vector3(1.f, 0.f, 0.f));
}

inline Vector3 Quat::GetAxisY() const
{
	return RotateVector(Vector3(0.f, 1.f, 0.f));
}

inline Vector3 Quat::GetAxisZ() const
{
	return RotateVector(Vector3(0.f, 0.f, 1.f));
}

inline Vector3 Quat::GetForwardVector() const
{
	return GetAxisX();
}

inline Vector3 Quat::GetRightVector() const
{
	return GetAxisY();
}

inline Vector3 Quat::GetUpVector() const
{
	return GetAxisZ();
}

inline Vector3 Quat::GetVector() const
{
	return GetAxisX();
}

inline void Quat::ToMatrix(Matrix4x4& outMatrix) const
{
	const float x2 = x + x;  const float y2 = y + y;  const float z2 = z + z;
	const float xx = x * x2; const float xy = x * y2; const float xz = x * z2;
	const float yy = y * y2; const float yz = y * z2; const float zz = z * z2;
	const float wx = w * x2; const float wy = w * y2; const float wz = w * z2;

	outMatrix.m[0][0] = 1.0f - (yy + zz);	outMatrix.m[1][0] = xy - wz;				outMatrix.m[2][0] = xz + wy;			outMatrix.m[3][0] = 0;
	outMatrix.m[0][1] = xy + wz;			outMatrix.m[1][1] = 1.0f - (xx + zz);		outMatrix.m[2][1] = yz - wx;			outMatrix.m[3][1] = 0;
	outMatrix.m[0][2] = xz - wy;			outMatrix.m[1][2] = yz + wx;				outMatrix.m[2][2] = 1.0f - (xx + yy);	outMatrix.m[3][2] = 0;
	outMatrix.m[0][3] = 0.0f;				outMatrix.m[1][3] = 0.0f;					outMatrix.m[2][3] = 0.0f;				outMatrix.m[3][3] = 1.0f;
}

inline Matrix4x4 Quat::ToMatrix() const
{
	Matrix4x4 matrix;
	ToMatrix(matrix);
	return matrix;
}

inline bool Quat::ContainsNaN() const
{
	return (
		!MMath::IsFinite(x) ||
		!MMath::IsFinite(y) ||
		!MMath::IsFinite(z) ||
		!MMath::IsFinite(w)
	);
}