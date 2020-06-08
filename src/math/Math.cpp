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

#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quat.h"

// ---------------------------------------- Globals ----------------------------------------

static int32 G_SRandSeed;

// Vector2
const Vector2 Vector2::ZeroVector(0.0f, 0.0f);
const Vector2 Vector2::UnitVector(1.0f, 1.0f);

// Vector3
const Vector3 Vector3::ZeroVector(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::OneVector(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::UpVector(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ForwardVector(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::RightVector(1.0f, 0.0f, 0.0f);
// Vector4

// quat
const Quat Quat::Identity(0, 0, 0, 1);

// matrix
const Matrix4x4 Matrix4x4::Identity(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(0, 0, 0, 1));

// ---------------------------------------- Math ----------------------------------------
Vector2 MMath::RandPointInCircle(float circleRadius)
{
	Vector2 point;
	float l;
	do
	{
		point.x = FRand() * 2.f - 1.f;
		point.x = FRand() * 2.f - 1.f;
		l = point.SizeSquared();
	} while (l > 1.0f);

	return point * circleRadius;
}

float MMath::GetRangeValue(const Vector2& range, float pct)
{
	return Lerp<float>(range.x, range.y, pct);
}

float MMath::InterpTo(float current, float target, float deltaTime, float interpSpeed)
{
	if (interpSpeed <= 0.f) {
		return target;
	}

	const float dist = target - current;
	if (MMath::Square(dist) < SMALL_NUMBER) {
		return target;
	}

	const float deltaMove = dist * MMath::Clamp<float>(deltaTime * interpSpeed, 0.f, 1.f);
	return current + deltaMove;
}

void MMath::SRandInit(int32 seed)
{
	G_SRandSeed = seed;
}

int32 MMath::GetRandSeed()
{
	return G_SRandSeed;
}

float MMath::SRand()
{
	G_SRandSeed = (G_SRandSeed * 196314165) + 907633515;

	union 
	{ 
		float f; 
		int32 i;
	} result;

	union 
	{ 
		float f; 
		int32 i;
	} temp;

	const float randTemp = 1.0f;
	temp.f   = randTemp;
	result.i = (temp.i & 0xff800000) | (G_SRandSeed & 0x007fffff);

	return Fractional(result.f);
}

float MMath::Atan2(float y, float x)
{
	const float absX = Abs(x);
	const float absY = Abs(y);
	const bool yAbsBigger = (absY > absX);
	float t0 = yAbsBigger ? absY : absX;
	float t1 = yAbsBigger ? absX : absY;

	if (t0 == 0.f) {
		return 0.f;
	}

	float t3 = t1 / t0;
	float t4 = t3 * t3;

	static const float c[7] = {
		+7.2128853633444123e-03f,
		-3.5059680836411644e-02f,
		+8.1675882859940430e-02f,
		-1.3374657325451267e-01f,
		+1.9856563505717162e-01f,
		-3.3324998579202170e-01f,
		+1.0f
	};

	t0 = c[0];
	t0 = t0 * t4 + c[1];
	t0 = t0 * t4 + c[2];
	t0 = t0 * t4 + c[3];
	t0 = t0 * t4 + c[4];
	t0 = t0 * t4 + c[5];
	t0 = t0 * t4 + c[6];
	t3 = t0 * t3;

	t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
	t3 = (x < 0.0f) ? PI - t3 : t3;
	t3 = (y < 0.0f) ? -t3 : t3;

	return t3;
}


// ---------------------------------------- Vector3 ----------------------------------------

Vector3::Vector3(const Vector4& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
{

}

// ---------------------------------------- Matrix4x4 ----------------------------------------

Quat Matrix4x4::ToQuat() const
{
	Quat result(*this);
	return result;
}