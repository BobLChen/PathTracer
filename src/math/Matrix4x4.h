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

#include "math/Math.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Axis.h"

struct Matrix4x4
{
public:

	enum Style 
	{
		EulerAngles,
		AxisAngle,
		Quaternion
	};

	float m[4][4];

	static const Matrix4x4 Identity;

public:
	Matrix4x4();

	Matrix4x4(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inW);

	inline void Perspective(float halfFOV, float width, float height, float minZ, float maxZ);

	inline void Orthographic(float left, float right, float bottom, float top, float minZ, float maxZ);

	inline void SetIdentity();

	inline void Prepend(const Matrix4x4& other);

	inline void Append(const Matrix4x4& other);

	inline void PrependRotation(const float degrees, const Vector3& axis, const Vector3* pivot = nullptr);

	inline void AppendRotation(const float degrees, const Vector3& axis, const Vector3* pivot = nullptr);
	
	inline void AppendTranslation(const Vector3& translation);

	inline void PrependTranslation(const Vector3& translation);

	inline void TranslateAxis(const Vector3& axis, float distance);

	inline void TranslateX(float distance);

	inline void TranslateY(float distance);

	inline void TranslateZ(float distance);

	inline void RotateAxis(const Vector3& axis, float angle, const Vector3* pivot);

	inline void RotateX(float angle, bool local = true, const Vector3* pivot = nullptr);

	inline void RotateY(float angle, bool local = true, const Vector3* pivot = nullptr);

	inline void RotateZ(float angle, bool local = true, const Vector3* pivot = nullptr);
	
	inline void AppendScale(const Vector3& scale);

	inline void PrependScale(const Vector3& scale);
	
	inline void CopyColumnFrom(int32 column, const Vector4 &vec);

	inline void CopyColumnTo(int32 column, Vector4 &vec) const;

	inline void Decompose(Style style, Vector4& outPos, Vector4& outScale, Vector4& outRot) const;

	inline void Recompose(const Vector4& pos, const Vector4& scale, const Vector4& rot);

	inline Vector4 DeltaTransformVector(const Vector4& v) const;

	inline void CopyRawDataTo(float* rawData) const;

	inline void CopyRawDataFrom(float* rawData);

	inline void SetOrientation(const Vector3& dir, const Vector3* up, float smooth);

	inline void LookAt(float x, float y, float z, const Vector3* up = nullptr, float smooth = 1.0f);

	inline void LookAt(Vector3 target, const Vector3* up = nullptr, float smooth = 1.0f);

	inline void CopyRawFrom(int32 column, const Vector4 &vec);

	inline void CopyRawTo(int32 column, Vector4 &vec) const;
	
	inline Matrix4x4 operator* (const Matrix4x4& other) const;

	inline void operator*=(const Matrix4x4& other);

	inline Matrix4x4 operator+ (const Matrix4x4& other) const;

	inline void operator+=(const Matrix4x4& other);

	inline Matrix4x4 operator* (float other) const;

	inline void operator*=(float other);

	inline bool operator==(const Matrix4x4& other) const;

	inline bool Equals(const Matrix4x4& other, float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool operator!=(const Matrix4x4& other) const;

	inline Vector4 TransformVector4(const Vector4& v) const;

	inline Vector4 TransformPosition(const Vector3 &v) const;

	inline Vector3 InverseTransformPosition(const Vector3 &v) const;

	inline Vector4 TransformVector(const Vector3& v) const;

	inline Vector3 InverseTransformVector(const Vector3 &v) const;

	inline Matrix4x4 GetTransposed() const;

	inline void SetTransposed();

	inline float Determinant() const;

	inline float RotDeterminant() const;

	inline Matrix4x4 InverseFast() const;

	inline void SetInverseFast();

	inline Matrix4x4 Inverse() const;

	inline void SetInverse();

	inline Matrix4x4 TransposeAdjoint() const;

	inline void RemoveScaling(float tolerance = SMALL_NUMBER);

	inline Matrix4x4 GetMatrixWithoutScale(float tolerance = SMALL_NUMBER) const;

	inline Vector3 ExtractScaling(float tolerance = SMALL_NUMBER);

	inline Vector3 GetScaleVector(float tolerance = SMALL_NUMBER) const;

	inline Vector3 GetRotation() const;

	inline void SetRotation(const Vector3& rotation);

	inline Vector3 GetScale() const;

	inline void ScaleX(float scale);

	inline void ScaleY(float scale);

	inline void ScaleZ(float scale);

	inline void SetScale(const Vector3& scale, float smooth = 1.0f);

	inline Matrix4x4 RemoveTranslation() const;

	inline Matrix4x4 ConcatTranslation(const Vector3& translation) const;

	inline bool ContainsNaN() const;

	inline void ScaleTranslation(const Vector3& scale3D);

	inline float GetMaximumAxisScale() const;

	inline Vector3 GetOrigin() const;

	inline Vector3 GetScaledAxis(Axis::Type axis) const;

	inline void GetScaledAxes(Vector3& x, Vector3& y, Vector3& z) const;

	inline Vector3 GetUnitAxis(Axis::Type axis) const;

	inline void GetUnitAxes(Vector3& x, Vector3& y, Vector3& z) const;

	inline void SetAxis(int32 i, const Vector3& axis);

	inline void SetOrigin(const Vector3& newOrigin);

	inline void SetPosition(const Vector3& pos);

	inline void SetAxes(Vector3* axis0 = nullptr, Vector3* axis1 = nullptr, Vector3* axis2 = nullptr, Vector3* origin = nullptr);

	inline Vector3 GetColumn(int32 col) const;

	inline Vector3 GetRight() const;

	inline Vector3 GetUp() const;

	inline Vector3 GetForward() const;

	inline Vector3 GetLeft() const;

	inline Vector3 GetBackward() const;

	inline Vector3 GetDown() const;

	inline void Mirror(Axis::Type mirrorAxis, Axis::Type flipAxis);

	inline void To3x4MatrixTranspose(float* Out) const;

	Quat ToQuat() const;

	inline static void GetAxisRotation(float u, float v, float w, const float a, const float b, const float c, const float degress, Matrix4x4& m);
};

inline Matrix4x4::Matrix4x4()
{
	m[0][0] = 1; m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
}

inline Matrix4x4::Matrix4x4(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inW)
{
	m[0][0] = inX.x; m[0][1] = inX.y;  m[0][2] = inX.z;  m[0][3] = 0.0f;
	m[1][0] = inY.x; m[1][1] = inY.y;  m[1][2] = inY.z;  m[1][3] = 0.0f;
	m[2][0] = inZ.x; m[2][1] = inZ.y;  m[2][2] = inZ.z;  m[2][3] = 0.0f;
	m[3][0] = inW.x; m[3][1] = inW.y;  m[3][2] = inW.z;  m[3][3] = 1.0f;
}

inline void Matrix4x4::To3x4MatrixTranspose(float* out) const
{
	const float* src = &(m[0][0]);
	float* dest = out;

	dest[0] = src[0];   // [0][0]
	dest[1] = src[4];   // [1][0]
	dest[2] = src[8];   // [2][0]
	dest[3] = src[12];  // [3][0]

	dest[4] = src[1];   // [0][1]
	dest[5] = src[5];   // [1][1]
	dest[6] = src[9];   // [2][1]
	dest[7] = src[13];  // [3][1]

	dest[8] = src[2];   // [0][2]
	dest[9] = src[6];   // [1][2]
	dest[10] = src[10]; // [2][2]
	dest[11] = src[14]; // [3][2]
}

inline void Matrix4x4::SetIdentity()
{
	m[0][0] = 1; m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
}

inline void Matrix4x4::PrependRotation(const float degrees, const Vector3& axis, const Vector3* pivot)
{
	Matrix4x4 matrix;
	if (pivot) {
		GetAxisRotation(axis.x, axis.y, axis.z, pivot->x, pivot->y, pivot->z, degrees, matrix);
	}
	else {
		GetAxisRotation(axis.x, axis.y, axis.z, 0, 0, 0, degrees, matrix);
	}
	Prepend(matrix);
}

inline void Matrix4x4::AppendRotation(const float degrees, const Vector3& axis, const Vector3* pivot)
{
	Matrix4x4 matrix;
	if (pivot) {
		GetAxisRotation(axis.x, axis.y, axis.z, pivot->x, pivot->y, pivot->z, degrees, matrix);
	}
	else {
		GetAxisRotation(axis.x, axis.y, axis.z, 0, 0, 0, degrees, matrix);
	}
	Append(matrix);
}

inline void Matrix4x4::AppendTranslation(const Vector3& translation)
{
	m[3][0] += translation.x; 
	m[3][1] += translation.y;
	m[3][2] += translation.z;
}

inline void Matrix4x4::TranslateY(float distance)
{
	Vector4 pos;
	Vector4 up;
	CopyRawTo(3, pos);
	CopyRawTo(1, up);
	pos.x += distance * up.x;
	pos.y += distance * up.y;
	pos.z += distance * up.z;
	CopyRawFrom(3, pos);
}

inline void Matrix4x4::TranslateZ(float distance)
{
	Vector4 pos;
	Vector4 dir;
	CopyRawTo(3, pos);
	CopyRawTo(2, dir);
	pos.x += distance * dir.x;
	pos.y += distance * dir.y;
	pos.z += distance * dir.z;
	CopyRawFrom(3, pos);
}

inline void Matrix4x4::TranslateAxis(const Vector3& axis, float distance)
{
	Vector4 pos;
	CopyRawTo(3, pos);
	pos.x += distance * axis.x;
	pos.y += distance * axis.y;
	pos.z += distance * axis.z;
	CopyRawFrom(3, pos);
}

inline void Matrix4x4::TranslateX(float distance)
{
	Vector4 pos;
	Vector4 right;
	CopyRawTo(3, pos);
	CopyRawTo(0, right);
	pos.x += distance * right.x;
	pos.y += distance * right.y;
	pos.z += distance * right.z;
	CopyRawFrom(3, pos);
}

inline void Matrix4x4::GetAxisRotation(float u, float v, float w, const float a, const float b, const float c, const float degress, Matrix4x4& m)
{
	float rad = degress / 180.0f * PI;

	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;
	float l2 = u2 + v2 + w2;
	float l = MMath::Sqrt(l2);

	u /= l;
	v /= l;
	w /= l;
	u2 /= l2;
	v2 /= l2;
	w2 /= l2;

	float cos = MMath::Cos(rad);
	float sin = MMath::Sin(rad);

	m.m[0][0] = u2 + (v2 + w2) * cos;
	m.m[0][1] = u * v * (1 - cos) + w * sin;
	m.m[0][2] = u * w * (1 - cos) - v * sin;
	m.m[0][3] = 0;

	m.m[1][0] = u * v * (1 - cos) - w * sin;
	m.m[1][1] = v2 + (u2 + w2) * cos;
	m.m[1][2] = v * w * (1 - cos) + u * sin;
	m.m[1][3] = 0;

	m.m[2][0] = u * w * (1 - cos) + v * sin;
	m.m[2][1] = v * w * (1 - cos) - u * sin;
	m.m[2][2] = w2 + (u2 + v2) * cos;
	m.m[2][3] = 0;

	m.m[3][0] = (a * (v2 + w2) - u * (b * v + c * w)) * (1 - cos) + (b * w - c * v) * sin;
	m.m[3][1] = (b * (u2 + w2) - v * (a * u + c * w)) * (1 - cos) + (c * u - a * w) * sin;
	m.m[3][2] = (c * (u2 + v2) - w * (a * u + b * v)) * (1 - cos) + (a * v - b * u) * sin;
	m.m[3][3] = 1;
}

inline void Matrix4x4::RotateX(float angle, bool local, const Vector3* pivot)
{
	if (local) {
		Vector3 vec = GetRight();
		RotateAxis(vec, angle, pivot);
	}
	else {
		RotateAxis(Vector3::RightVector, angle, pivot);
	}
}

inline void Matrix4x4::RotateY(float angle, bool local, const Vector3* pivot)
{
	if (local) {
		Vector3 vec = GetUp();
		RotateAxis(vec, angle, pivot);
	}
	else {
		RotateAxis(Vector3::UpVector, angle, pivot);
	}
}

inline void Matrix4x4::RotateZ(float angle, bool local, const Vector3* pivot)
{
	if (local) {
		Vector3 vec = GetForward();
		RotateAxis(vec, angle, pivot);
	}
	else {
		RotateAxis(Vector3::ForwardVector, angle, pivot);
	}
}

inline void Matrix4x4::RotateAxis(const Vector3& axis, float angle, const Vector3* pivot)
{
	Vector3 vec;
	vec.x = axis.x;
	vec.y = axis.y;
	vec.z = axis.z;
	vec.Normalize();

	if (pivot) {
		AppendRotation(angle, vec, pivot);
	}
	else {
		Vector4 pos;
		CopyRawTo(3, pos);
		Vector3 pos3(pos.x, pos.y, pos.z);
		AppendRotation(angle, vec, &pos3);
	}
}

inline void Matrix4x4::PrependTranslation(const Vector3& translation)
{
	Matrix4x4 matrix;
	matrix.SetOrigin(translation);
	Prepend(matrix);
}

inline void Matrix4x4::AppendScale(const Vector3& scale)
{
	Matrix4x4 matrix;
	matrix.SetIdentity();
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	Append(matrix);
}

inline void Matrix4x4::PrependScale(const Vector3& scale)
{
	Matrix4x4 matrix;
	matrix.SetIdentity();
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	Prepend(matrix);
}

inline void Matrix4x4::Prepend(const Matrix4x4& other)
{
	MMath::VectorMatrixMultiply(this, &other, this);
}

inline void Matrix4x4::Append(const Matrix4x4& other)
{
	MMath::VectorMatrixMultiply(this, this, &other);
}

inline void Matrix4x4::CopyColumnFrom(int32 column, const Vector4 &vec)
{
	m[0][column] = vec.x; 
	m[1][column] = vec.y; 
	m[2][column] = vec.z; 
	m[3][column] = vec.w;
}

inline void Matrix4x4::CopyColumnTo(int32 column, Vector4 &vec) const
{
	vec.x = m[0][column];
	vec.y = m[1][column];
	vec.z = m[2][column];
	vec.w = m[3][column];
}

inline void Matrix4x4::CopyRawDataFrom(float* rawData)
{
	m[0][0] = rawData[0];	m[0][1] = rawData[1];	m[0][2] = rawData[2];	m[0][3] = rawData[3];
	m[1][0] = rawData[4];	m[1][1] = rawData[5];	m[1][2] = rawData[6];	m[1][3] = rawData[7];
	m[2][0] = rawData[8];	m[2][1] = rawData[9];	m[2][2] = rawData[10];	m[2][3] = rawData[11];
	m[3][0] = rawData[12];	m[3][1] = rawData[13];	m[3][2] = rawData[14];	m[3][3] = rawData[15];
}

inline void Matrix4x4::CopyRawDataTo(float* rawData) const
{
	rawData[0] = m[0][0];
	rawData[1] = m[0][1];
	rawData[2] = m[0][2];
	rawData[3] = m[0][3];

	rawData[4] = m[1][0];
	rawData[5] = m[1][1];
	rawData[6] = m[1][2];
	rawData[7] = m[1][3];

	rawData[8] = m[2][0];
	rawData[9] = m[2][1];
	rawData[10] = m[2][2];
	rawData[11] = m[2][3];

	rawData[12] = m[3][0];
	rawData[13] = m[3][1];
	rawData[14] = m[3][2];
	rawData[15] = m[3][3];
}

inline Vector4 Matrix4x4::DeltaTransformVector(const Vector4& v) const
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	return Vector4(
		(x * m[0][0] + y * m[1][0] + z * m[2][0]),
		(x * m[0][1] + y * m[1][1] + z * m[2][1]),
		(x * m[0][2] + y * m[1][2] + z * m[2][2]),
		(x * m[0][3] + y * m[1][3] + z * m[2][3])
	);
}

inline void Matrix4x4::Recompose(const Vector4& pos, const Vector4& scale, const Vector4& rot)
{
	SetIdentity();
	AppendScale(scale);

	Matrix4x4 temp;

	float angle = -rot.x;
	float v[] = { 1, 0, 0, 0, 0, MMath::Cos(angle), -MMath::Sin(angle), 0, 0, MMath::Sin(angle), MMath::Cos(angle), 0, 0, 0, 0 , 0 };
	temp.CopyRawDataFrom(v);
	Append(temp);
	
	angle = -rot.y;
	float v0[] = {MMath::Cos(angle), 0, MMath::Sin(angle), 0, 0, 1, 0, 0, -MMath::Sin(angle), 0, MMath::Cos(angle), 0, 0, 0, 0, 0};
	temp.CopyRawDataFrom(v0);
	Append(temp);

	angle = -rot.z;
	float v1[] = {MMath::Cos(angle), -MMath::Sin(angle), 0, 0, MMath::Sin(angle), MMath::Cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	temp.CopyRawDataFrom(v1);
	Append(temp);

	SetPosition(pos);
	m[3][3] = 1.0f;
}

inline void Matrix4x4::Decompose(Style style, Vector4& outPos, Vector4& outScale, Vector4& outRot) const
{
	float mr[16] = { 0.0f };
	CopyRawDataTo(mr);

	// postion
	outPos.x = mr[12];
	outPos.y = mr[13];
	outPos.z = mr[14];
	outPos.w = 1.0f;

	mr[12] = mr[13] = mr[14] = 0.0f;

	// scale
	outScale.x = MMath::Sqrt(mr[0] * mr[0] + mr[1] * mr[1] + mr[2] * mr[2]);
	outScale.y = MMath::Sqrt(mr[4] * mr[4] + mr[5] * mr[5] + mr[6] * mr[6]);
	outScale.z = MMath::Sqrt(mr[8] * mr[8] + mr[9] * mr[9] + mr[10] * mr[10]);
	outScale.w = 1.0f;

	if (mr[0] * (mr[5] * mr[10] - mr[6] * mr[9]) - mr[1] * (mr[4] * mr[10] - mr[6] * mr[8]) + mr[2] * (mr[4] * mr[9] - mr[5] * mr[8]) < 0) 
	{
		outScale.z = -outScale.z;
	}
	
	mr[0]  /= outScale.x;
	mr[1]  /= outScale.x;
	mr[2]  /= outScale.x;
	mr[4]  /= outScale.y;
	mr[5]  /= outScale.y;
	mr[6]  /= outScale.y;
	mr[8]  /= outScale.z;
	mr[9]  /= outScale.z;
	mr[10] /= outScale.z;

	if (style == Style::EulerAngles) 
	{
		outRot.y = MMath::Asin(-mr[2]);
		if (mr[2] != 1 && mr[2] != -1) 
		{
			outRot.x = MMath::Atan2(mr[6], mr[10]);
			outRot.z = MMath::Atan2(mr[1], mr[0]);
			
		} 
		else 
		{
			outRot.z = 0;
			outRot.x = MMath::Atan2(mr[4], mr[5]);
		}
	} 
	else if (style == Style::AxisAngle) 
	{
		outRot.w = MMath::Acos((mr[0] + mr[5] + mr[10] - 1) / 2);
		float len = MMath::Sqrt((mr[6] - mr[9]) * (mr[6] - mr[9]) + (mr[8] - mr[2]) * (mr[8] - mr[2]) + (mr[1] - mr[4]) * (mr[1] - mr[4]));
		outRot.x = (mr[6] - mr[9]) / len;
		outRot.y = (mr[8] - mr[2]) / len;
		outRot.z = (mr[1] - mr[4]) / len;
	} 
	else if (style == Style::Quaternion) 
	{
		float tr = mr[0] + mr[5] + mr[10];
		if (tr > 0) 
		{
			outRot.w = MMath::Sqrt(1 + tr) / 2;
			outRot.x = (mr[6] - mr[9]) / (4 * outRot.w);
			outRot.y = (mr[8] - mr[2]) / (4 * outRot.w);
			outRot.z = (mr[1] - mr[4]) / (4 * outRot.w);
		} 
		else if ((mr[0] > mr[5]) && (mr[0] > mr[10])) {
			outRot.x = MMath::Sqrt(1 + mr[0] - mr[5] - mr[10])/2;
			outRot.w = (mr[6] - mr[9]) / (4 * outRot.x);
			outRot.y = (mr[1] + mr[4]) / (4 * outRot.x);
			outRot.z = (mr[8] + mr[2]) / (4 * outRot.x);
		} 
		else {
			outRot.z = MMath::Sqrt(1 + mr[10] - mr[0] - mr[5])/2;
			outRot.x = (mr[8] + mr[2]) / (4 * outRot.z);
			outRot.y = (mr[6] + mr[9]) / (4 * outRot.z);
			outRot.w = (mr[1] - mr[4]) / (4 * outRot.z);
		}
	}
}

inline void Matrix4x4::LookAt(float x, float y, float z, const Vector3* up, float smooth)
{
	Vector3 vector(x - m[3][0], y - m[3][1], z - m[3][2]);
	SetOrientation(vector, up, smooth);
}

inline void Matrix4x4::LookAt(Vector3 target, const Vector3* up, float smooth)
{
	LookAt(target.x, target.y, target.z, up, smooth);
}

inline void Matrix4x4::SetOrientation(const Vector3& dir, const Vector3* up, float smooth)
{
	Vector3 scale;

	Vector4 vec;
	CopyRawTo(0, vec);
	scale.x = vec.Size3();
	CopyRawTo(1, vec);
	scale.y = vec.Size3();
	CopyRawTo(2, vec);
	scale.z = vec.Size3();

	Vector3 tempDir = dir;
	tempDir.Normalize();

	Vector3 tempUP;
	if (up == nullptr)
	{
		if (tempDir.x == 0.0f && MMath::Abs(tempDir.y) == 1 && tempDir.z == 0) {
			tempUP = Vector3::ForwardVector;
		}
		else {
			tempUP = Vector3::UpVector;
		}
	}
	else
	{
		tempUP = *up;
	}

	if (smooth != 1.0f)
	{
		CopyRawTo(2, vec);
		vec.x = (vec.x + ((tempDir.x - vec.x) * smooth));
		vec.y = (vec.y + ((tempDir.y - vec.y) * smooth));
		vec.z = (vec.z + ((tempDir.z - vec.z) * smooth));
		tempDir = vec;

		CopyRawTo(1, vec);
		vec.x = (vec.x + ((tempUP.x - vec.x) * smooth));
		vec.y = (vec.y + ((tempUP.y - vec.y) * smooth));
		vec.z = (vec.z + ((tempUP.z - vec.z) * smooth));
		tempUP = vec;
	}

	tempDir.Normalize();
	tempUP.Normalize();

	Vector3 rVec = Vector3::CrossProduct(tempUP, tempDir);
	rVec.Normalize();

	Vector3 uVec = Vector3::CrossProduct(tempDir, rVec);

	rVec.Scale(scale.x);
	uVec.Scale(scale.y);
	tempDir.Scale(scale.z);
    
	CopyRawFrom(0, Vector4(rVec, 0.0f));
	CopyRawFrom(1, Vector4(uVec, 0.0f));
	CopyRawFrom(2, Vector4(tempDir, 0.0f));
}

inline void Matrix4x4::CopyRawFrom(int32 raw, const Vector4 &vec)
{
	m[raw][0] = vec.x;
    m[raw][1] = vec.y;
    m[raw][2] = vec.z;
    m[raw][3] = vec.w;
}

inline void Matrix4x4::CopyRawTo(int32 raw, Vector4 &vec) const
{
	vec.x = m[raw][0];
	vec.y = m[raw][1];
	vec.z = m[raw][2];
	vec.w = m[raw][3];
}

inline void Matrix4x4::operator*=(const Matrix4x4& other)
{
    MMath::VectorMatrixMultiply(this, this, &other);
}

inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	Matrix4x4 result;
	MMath::VectorMatrixMultiply(&result, this, &other);
	return result;
}

inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
	Matrix4x4 resultMat;

	for (int32 x = 0; x < 4; x++)
	{
		for (int32 y = 0; y < 4; y++) {
			resultMat.m[x][y] = m[x][y] + other.m[x][y];
		}
	}

	return resultMat;
}

inline void Matrix4x4::operator+=(const Matrix4x4& other)
{
	*this = *this + other;
}

inline Matrix4x4 Matrix4x4::operator*(float other) const
{
	Matrix4x4 resultMat;

	for (int32 x = 0; x < 4; x++)
	{
		for (int32 y = 0; y < 4; y++) {
			resultMat.m[x][y] = m[x][y] * other;
		}
	}

	return resultMat;
}

inline void Matrix4x4::operator*=(float other)
{
	*this = *this * other;
}

inline bool Matrix4x4::operator==(const Matrix4x4& other) const
{
	for (int32 x = 0; x < 4; x++)
	{
		for (int32 y = 0; y < 4; y++)
		{
			if (m[x][y] != other.m[x][y]) {
				return false;
			}
		}
	}

	return true;
}

inline bool Matrix4x4::Equals(const Matrix4x4& other, float tolerance) const
{
	for (int32 x = 0; x < 4; x++)
	{
		for (int32 y = 0; y < 4; y++)
		{
			if (MMath::Abs(m[x][y] - other.m[x][y]) > tolerance) {
				return false;
			}
		}
	}

	return true;
}

inline bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
	return !(*this == other);
}

inline Vector4 Matrix4x4::TransformVector4(const Vector4 &v) const
{
    Vector4 result;
    MMath::VectorTransformVector(&result, &v, this);
	return result;
}

inline Vector4 Matrix4x4::TransformPosition(const Vector3 &v) const
{
	return TransformVector4(Vector4(v.x, v.y, v.z, 1.0f));
}

inline Vector3 Matrix4x4::InverseTransformPosition(const Vector3 &v) const
{
	Matrix4x4 invSelf = this->InverseFast();
	return invSelf.TransformPosition(v);
}

inline Vector4 Matrix4x4::TransformVector(const Vector3& v) const
{
	Vector4 col0;
	Vector4 col1;
	Vector4 col2;
	Vector4 row3;
	CopyColumnTo(0, col0);
	CopyColumnTo(1, col1);
	CopyColumnTo(2, col2);
	CopyRawTo(3, row3);

	Vector4 temp;
	temp.x = row3.x + v.x * col0.x + v.y * col0.y + v.z * col0.z;
	temp.y = row3.y + v.x * col1.x + v.y * col1.y + v.z * col1.z;
	temp.z = row3.z + v.x * col2.x + v.y * col2.y + v.z * col2.z;
	temp.w = 1.0f;

	return temp;
}

inline Vector3 Matrix4x4::InverseTransformVector(const Vector3 &v) const
{
	Matrix4x4 invSelf = this->InverseFast();
	return invSelf.TransformVector(v);
}

inline void Matrix4x4::SetTransposed()
{
	*this = GetTransposed();
}

inline Matrix4x4 Matrix4x4::GetTransposed() const
{
	Matrix4x4 result;

	result.m[0][0] = m[0][0];
	result.m[0][1] = m[1][0];
	result.m[0][2] = m[2][0];
	result.m[0][3] = m[3][0];

	result.m[1][0] = m[0][1];
	result.m[1][1] = m[1][1];
	result.m[1][2] = m[2][1];
	result.m[1][3] = m[3][1];

	result.m[2][0] = m[0][2];
	result.m[2][1] = m[1][2];
	result.m[2][2] = m[2][2];
	result.m[2][3] = m[3][2];

	result.m[3][0] = m[0][3];
	result.m[3][1] = m[1][3];
	result.m[3][2] = m[2][3];
	result.m[3][3] = m[3][3];

	return result;
}

inline float Matrix4x4::Determinant() const
{
	return	m[0][0] * 
		(
			m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
		) -
		m[1][0] * 
		(
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
		) +
		m[2][0] * 
		(
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
		) -
		m[3][0] * 
		(
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
		);
}

inline float Matrix4x4::RotDeterminant() const
{
	return
		m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
		m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
}

inline void Matrix4x4::SetInverseFast()
{
	*this = InverseFast();
}

inline Matrix4x4 Matrix4x4::InverseFast() const
{
	Matrix4x4 result;
    MMath::VectorMatrixInverse(&result, this);
	return result;
}

inline void Matrix4x4::SetInverse()
{
	*this = Inverse();
}

inline Matrix4x4 Matrix4x4::Inverse() const
{
	Matrix4x4 result;

	if (GetScaledAxis(Axis::X).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(Axis::Y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(Axis::Z).IsNearlyZero(SMALL_NUMBER))
	{
		result = Matrix4x4::Identity;
	}
	else
	{
		const float	Det = Determinant();

		if (Det == 0.0f) {
			result = Matrix4x4::Identity;
		}
		else {
			MMath::VectorMatrixInverse(&result, this);
		}
	}

	return result;
}

inline Matrix4x4 Matrix4x4::TransposeAdjoint() const
{
	Matrix4x4 ta;

	ta.m[0][0] = this->m[1][1] * this->m[2][2] - this->m[1][2] * this->m[2][1];
	ta.m[0][1] = this->m[1][2] * this->m[2][0] - this->m[1][0] * this->m[2][2];
	ta.m[0][2] = this->m[1][0] * this->m[2][1] - this->m[1][1] * this->m[2][0];
	ta.m[0][3] = 0.f;

	ta.m[1][0] = this->m[2][1] * this->m[0][2] - this->m[2][2] * this->m[0][1];
	ta.m[1][1] = this->m[2][2] * this->m[0][0] - this->m[2][0] * this->m[0][2];
	ta.m[1][2] = this->m[2][0] * this->m[0][1] - this->m[2][1] * this->m[0][0];
	ta.m[1][3] = 0.f;

	ta.m[2][0] = this->m[0][1] * this->m[1][2] - this->m[0][2] * this->m[1][1];
	ta.m[2][1] = this->m[0][2] * this->m[1][0] - this->m[0][0] * this->m[1][2];
	ta.m[2][2] = this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0];
	ta.m[2][3] = 0.f;

	ta.m[3][0] = 0.f;
	ta.m[3][1] = 0.f;
	ta.m[3][2] = 0.f;
	ta.m[3][3] = 1.f;

	return ta;
}

inline void Matrix4x4::RemoveScaling(float tolerance)
{
	const float squareSum0 = (m[0][0] * m[0][0]) + (m[0][1] * m[0][1]) + (m[0][2] * m[0][2]);
	const float squareSum1 = (m[1][0] * m[1][0]) + (m[1][1] * m[1][1]) + (m[1][2] * m[1][2]);
	const float squareSum2 = (m[2][0] * m[2][0]) + (m[2][1] * m[2][1]) + (m[2][2] * m[2][2]);
	const float scale0 = MMath::FloatSelect(squareSum0 - tolerance, MMath::InvSqrt(squareSum0), 1.0f);
	const float scale1 = MMath::FloatSelect(squareSum1 - tolerance, MMath::InvSqrt(squareSum1), 1.0f);
	const float scale2 = MMath::FloatSelect(squareSum2 - tolerance, MMath::InvSqrt(squareSum2), 1.0f);
	m[0][0] *= scale0;
	m[0][1] *= scale0;
	m[0][2] *= scale0;
	m[1][0] *= scale1;
	m[1][1] *= scale1;
	m[1][2] *= scale1;
	m[2][0] *= scale2;
	m[2][1] *= scale2;
	m[2][2] *= scale2;
}

inline Matrix4x4 Matrix4x4::GetMatrixWithoutScale(float tolerance) const
{
	Matrix4x4 result = *this;
	result.RemoveScaling(tolerance);
	return result;
}

inline Vector3 Matrix4x4::ExtractScaling(float tolerance)
{
	Vector3 scale3D(0, 0, 0);

	const float squareSum0 = (m[0][0] * m[0][0]) + (m[0][1] * m[0][1]) + (m[0][2] * m[0][2]);
	const float squareSum1 = (m[1][0] * m[1][0]) + (m[1][1] * m[1][1]) + (m[1][2] * m[1][2]);
	const float squareSum2 = (m[2][0] * m[2][0]) + (m[2][1] * m[2][1]) + (m[2][2] * m[2][2]);

	if (squareSum0 > tolerance)
	{
		float scale0 = MMath::Sqrt(squareSum0);
		scale3D[0] = scale0;
		float invScale0 = 1.f / scale0;
		m[0][0] *= invScale0;
		m[0][1] *= invScale0;
		m[0][2] *= invScale0;
	}
	else
	{
		scale3D[0] = 0;
	}

	if (squareSum1 > tolerance)
	{
		float scale1 = MMath::Sqrt(squareSum1);
		scale3D[1] = scale1;
		float InvScale1 = 1.f / scale1;
		m[1][0] *= InvScale1;
		m[1][1] *= InvScale1;
		m[1][2] *= InvScale1;
	}
	else
	{
		scale3D[1] = 0;
	}

	if (squareSum2 > tolerance)
	{
		float scale2 = MMath::Sqrt(squareSum2);
		scale3D[2] = scale2;
		float InvScale2 = 1.f / scale2;
		m[2][0] *= InvScale2;
		m[2][1] *= InvScale2;
		m[2][2] *= InvScale2;
	}
	else
	{
		scale3D[2] = 0;
	}

	return scale3D;
}

inline void Matrix4x4::ScaleX(float scale)
{
	Vector4 right;
	CopyRawTo(0, right);

	float length = right.Size3();
	right.x = right.x / length * scale;
	right.y = right.y / length * scale;
	right.z = right.z / length * scale;

	CopyRawFrom(0, right);
}

inline void Matrix4x4::ScaleY(float scale)
{
	Vector4 up;
	CopyRawTo(1, up);

	float length = up.Size3();
	up.x = up.x / length * scale;
	up.y = up.y / length * scale;
	up.z = up.z / length * scale;

	CopyRawFrom(1, up);
}

inline void Matrix4x4::ScaleZ(float scale)
{
	Vector4 dir;
	CopyRawTo(2, dir);

	float length = dir.Size3();
	dir.x = dir.x / length * scale;
	dir.y = dir.y / length * scale;
	dir.z = dir.z / length * scale;

	CopyRawFrom(2, dir);
}

inline void Matrix4x4::SetScale(const Vector3& scale, float smooth)
{
	Vector4 right;
	CopyRawTo(0, right);

	Vector4 up;
	CopyRawTo(1, up);

	Vector4 dir;
	CopyRawTo(2, dir);

	Vector3 temp(right.Size3(), up.Size3(), dir.Size3());

	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	temp.x += (x - temp.x) * smooth;
	temp.y += (y - temp.y) * smooth;
	temp.z += (z - temp.z) * smooth;

	right.x *= scale.x / x;
	right.y *= scale.x / x;
	right.z *= scale.x / x;

	up.x *= scale.y / y;
	up.y *= scale.y / y;
	up.z *= scale.y / y;

	dir.x *= scale.z / z;
	dir.y *= scale.z / z;
	dir.z *= scale.z / z;

	CopyRawFrom(0, right);
	CopyRawFrom(1, up);
	CopyRawFrom(2, dir);
}

inline void Matrix4x4::SetRotation(const Vector3& rotation)
{
	Vector4 pos;
	Vector4 rot;
	Vector4 scale;

	Decompose(Style::EulerAngles, pos, scale, rot);

	rot.x = MMath::DegreesToRadians(rotation.x);
	rot.y = MMath::DegreesToRadians(rotation.y);
	rot.z = MMath::DegreesToRadians(rotation.z);

	Recompose(pos, scale, rot);
}

inline Vector3 Matrix4x4::GetRotation() const
{
	Vector4 pos;
	Vector4 rot;
	Vector4 scale;

	Decompose(Style::EulerAngles, pos, scale, rot);

	return Vector3(MMath::RadiansToDegrees(rot.x), MMath::RadiansToDegrees(rot.y), MMath::RadiansToDegrees(rot.z));
}

inline Vector3 Matrix4x4::GetScale() const
{
	Vector4 temp;
	
	CopyRawTo(0, temp);
	float scaleX = temp.Size3();
	
	CopyRawTo(1, temp);
	float scaleY = temp.Size3();

	CopyRawTo(2, temp);
	float scaleZ = temp.Size3();

	return Vector3(scaleX, scaleY, scaleZ);
}

inline Vector3 Matrix4x4::GetScaleVector(float tolerance) const
{
	Vector3 scale3D(1, 1, 1);

	for (int32 i = 0; i < 3; i++)
	{
		const float SquareSum = (m[i][0] * m[i][0]) + (m[i][1] * m[i][1]) + (m[i][2] * m[i][2]);
		if (SquareSum > tolerance) {
			scale3D[i] = MMath::Sqrt(SquareSum);
		}
		else {
			scale3D[i] = 0.f;
		}
	}

	return scale3D;
}

inline Matrix4x4 Matrix4x4::RemoveTranslation() const
{
	Matrix4x4 result = *this;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	return result;
}

inline Matrix4x4 Matrix4x4::ConcatTranslation(const Vector3& translation) const
{
	Matrix4x4 result;

	float* dest = &result.m[0][0];
	const float* src = &m[0][0];
	const float* trans = &translation.x;

	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
	dest[4] = src[4];
	dest[5] = src[5];
	dest[6] = src[6];
	dest[7] = src[7];
	dest[8] = src[8];
	dest[9] = src[9];
	dest[10] = src[10];
	dest[11] = src[11];
	dest[12] = src[12] + trans[0];
	dest[13] = src[13] + trans[1];
	dest[14] = src[14] + trans[2];
	dest[15] = src[15];

	return result;
}

inline bool Matrix4x4::ContainsNaN() const
{
	for (int32 i = 0; i < 4; i++)
	{
		for (int32 j = 0; j < 4; j++)
		{
			if (!MMath::IsFinite(m[i][j])) {
				return true;
			}
		}
	}

	return false;
}

inline float Matrix4x4::GetMaximumAxisScale() const
{
	const float maxRowScaleSquared = MMath::Max(
		GetScaledAxis(Axis::X).SizeSquared(),
		MMath::Max(
			GetScaledAxis(Axis::Y).SizeSquared(),
			GetScaledAxis(Axis::Z).SizeSquared()
		)
	);
	return MMath::Sqrt(maxRowScaleSquared);
}

inline void Matrix4x4::ScaleTranslation(const Vector3& inScale3D)
{
	m[3][0] *= inScale3D.x;
	m[3][1] *= inScale3D.y;
	m[3][2] *= inScale3D.z;
}

inline Vector3 Matrix4x4::GetOrigin() const
{
	return Vector3(m[3][0], m[3][1], m[3][2]);
}

inline Vector3 Matrix4x4::GetScaledAxis(Axis::Type inAxis) const
{
	switch (inAxis)
	{
	case Axis::X:
		return Vector3(m[0][0], m[0][1], m[0][2]);

	case Axis::Y:
		return Vector3(m[1][0], m[1][1], m[1][2]);

	case Axis::Z:
		return Vector3(m[2][0], m[2][1], m[2][2]);

	default:
		return Vector3::ZeroVector;
	}
}

inline void Matrix4x4::GetScaledAxes(Vector3 &x, Vector3 &y, Vector3 &z) const
{
	x.x = m[0][0]; x.y = m[0][1]; x.z = m[0][2];
	y.x = m[1][0]; y.y = m[1][1]; y.z = m[1][2];
	z.x = m[2][0]; z.y = m[2][1]; z.z = m[2][2];
}

inline Vector3 Matrix4x4::GetUnitAxis(Axis::Type inAxis) const
{
	return GetScaledAxis(inAxis).GetSafeNormal();
}

inline void Matrix4x4::GetUnitAxes(Vector3 &x, Vector3 &y, Vector3 &z) const
{
	GetScaledAxes(x, y, z);
	x.Normalize();
	y.Normalize();
	z.Normalize();
}

inline void Matrix4x4::SetAxis(int32 i, const Vector3& axis)
{
	m[i][0] = axis.x;
	m[i][1] = axis.y;
	m[i][2] = axis.z;
}

inline void Matrix4x4::SetOrigin(const Vector3& newOrigin)
{
	m[3][0] = newOrigin.x;
	m[3][1] = newOrigin.y;
	m[3][2] = newOrigin.z;
}

inline void Matrix4x4::SetPosition(const Vector3& pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
}

inline void Matrix4x4::SetAxes(Vector3* axis0, Vector3* axis1, Vector3* axis2, Vector3* origin)
{
	if (axis0 != NULL)
	{
		m[0][0] = axis0->x;
		m[0][1] = axis0->y;
		m[0][2] = axis0->z;
	}
	if (axis1 != NULL)
	{
		m[1][0] = axis1->x;
		m[1][1] = axis1->y;
		m[1][2] = axis1->z;
	}
	if (axis2 != NULL)
	{
		m[2][0] = axis2->x;
		m[2][1] = axis2->y;
		m[2][2] = axis2->z;
	}
	if (origin != NULL)
	{
		m[3][0] = origin->x;
		m[3][1] = origin->y;
		m[3][2] = origin->z;
	}
}

inline Vector3 Matrix4x4::GetRight() const
{
	Vector4 right;
	CopyRawTo(0, right);
	return Vector3(right.x, right.y, right.z);
}

inline Vector3 Matrix4x4::GetUp() const
{
	Vector4 up;
	CopyRawTo(1, up);
	return Vector3(up.x, up.y, up.z);
}

inline Vector3 Matrix4x4::GetForward() const
{
	Vector4 forward;
	CopyRawTo(2, forward);
	return Vector3(forward.x, forward.y, forward.z);
}

inline Vector3 Matrix4x4::GetLeft() const
{
	Vector4 right;
	CopyRawTo(0, right);
	return Vector3(-right.x, -right.y, -right.z);
}

inline Vector3 Matrix4x4::GetBackward() const
{
	Vector4 forward;
	CopyRawTo(2, forward);
	return Vector3(-forward.x, -forward.y, -forward.z);
}

inline Vector3 Matrix4x4::GetDown() const
{
	Vector4 up;
	CopyRawTo(1, up);
	return Vector3(-up.x, -up.y, -up.z);
}

inline Vector3 Matrix4x4::GetColumn(int32 i) const
{
	return Vector3(m[0][i], m[1][i], m[2][i]);
}

inline void Matrix4x4::Mirror(Axis::Type mirrorAxis, Axis::Type flipAxis)
{
	if (mirrorAxis == Axis::X)
	{
		m[0][0] *= -1.f;
		m[1][0] *= -1.f;
		m[2][0] *= -1.f;
		m[3][0] *= -1.f;
	}
	else if (mirrorAxis == Axis::Y)
	{
		m[0][1] *= -1.f;
		m[1][1] *= -1.f;
		m[2][1] *= -1.f;
		m[3][1] *= -1.f;
	}
	else if (mirrorAxis == Axis::Z)
	{
		m[0][2] *= -1.f;
		m[1][2] *= -1.f;
		m[2][2] *= -1.f;
		m[3][2] *= -1.f;
	}

	if (flipAxis == Axis::X)
	{
		m[0][0] *= -1.f;
		m[0][1] *= -1.f;
		m[0][2] *= -1.f;
	}
	else if (flipAxis == Axis::Y)
	{
		m[1][0] *= -1.f;
		m[1][1] *= -1.f;
		m[1][2] *= -1.f;
	}
	else if (flipAxis == Axis::Z)
	{
		m[2][0] *= -1.f;
		m[2][1] *= -1.f;
		m[2][2] *= -1.f;
	}
}

inline void Matrix4x4::Perspective(float fovy, float width, float height, float zNear, float zFar)
{
	float aspect = width / height;
	float tanHalfFovy = MMath::Tan(fovy / 2);
	
	m[0][0] = 1.0f / (aspect * tanHalfFovy);	m[0][1] = 0.0f;					m[0][2] = 0.0f;										m[0][3] = 0.0f;
	m[1][0] = 0.0f;								m[1][1] = 1 / (tanHalfFovy);	m[1][2] = 0.0f;										m[1][3] = 0.0f;
	m[2][0] = 0.0f;								m[2][1] = 0.0f;					m[2][2] = -(zFar + zNear) / (zFar - zNear);			m[2][3] = -1.0f;
	m[3][0] = 0.0f;								m[3][1] = 0.0f;					m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);	m[3][3] = 0.0f;
}

inline void Matrix4x4::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m[0][0] = 2.0f / (right - left);			m[0][1] = 0.0f;								m[0][2] = 0.0f;								m[0][3] = 0.0f;
	m[1][0] = 0.0f;								m[1][1] = 2.0f / (top - bottom);			m[1][2] = 0.0f;								m[1][3] = 0.0f;
	m[2][0] = 0.0f;								m[2][1] = 0.0f;								m[2][2] = -2.0f / (zFar - zNear);			m[2][3] = 0.0f;
	m[3][0] = -(right + left) / (right - left);	m[3][1] = -(top + bottom) / (top - bottom);	m[3][2] = -(zFar + zNear) / (zFar - zNear);	m[3][3] = 1.0f;
}
