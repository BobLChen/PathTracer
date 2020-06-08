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
#include <cstring>
#include <vector>
#include <algorithm>
#include <math.h>

struct Vector2;
struct Vector3;
struct Vector4;
struct Quat;
struct Matrix4x4;

typedef unsigned char 		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef	signed char			int8;
typedef signed short int	int16;
typedef signed int	 		int32;
typedef signed long long	int64;

#undef  PI
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

#define MAX_FLT				3.402823466e+38F
#define INV_PI				(0.31830988618f)
#define HALF_PI				(1.57079632679f)
#define DELTA				(0.00001f)

#define MIN_uint8		((uint8)	0x00)
#define	MIN_uint16		((uint16)	0x0000)
#define	MIN_uint32		((uint32)	0x00000000)
#define MIN_uint64		((uint64)	0x0000000000000000)
#define MIN_int8		((int8)		-128)
#define MIN_int16		((int16)	-32768)
#define MIN_int32		((int32)	0x80000000)
#define MIN_int64		((int64)	0x8000000000000000)

#define MAX_uint8		((uint8)	0xff)
#define MAX_uint16		((uint16)	0xffff)
#define MAX_uint32		((uint32)	0xffffffff)
#define MAX_uint64		((uint64)	0xffffffffffffffff)
#define MAX_int8		((int8)		0x7f)
#define MAX_int16		((int16)	0x7fff)
#define MAX_int32		((int32)	0x7fffffff)
#define MAX_int64		((int64)	0x7fffffffffffffff)

#define MIN_flt			(1.175494351e-38F)			
#define MAX_flt			(3.402823466e+38F)
#define MIN_dbl			(2.2250738585072014e-308)	
#define MAX_dbl			(1.7976931348623158e+308)	

#define THRESH_POINT_ON_PLANE			(0.10f)		
#define THRESH_POINT_ON_SIDE			(0.20f)		
#define THRESH_POINTS_ARE_SAME			(0.00002f)	
#define THRESH_POINTS_ARE_NEAR			(0.015f)	
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	
#define THRESH_UVS_ARE_SAME			    (0.0009765625f)
#define THRESH_VECTORS_ARE_NEAR			(0.0004f)	
													
#define THRESH_SPLIT_POLY_WITH_PLANE	(0.25f)		
#define THRESH_SPLIT_POLY_PRECISELY		(0.01f)		
#define THRESH_ZERO_NORM_SQUARED		(0.0001f)	
#define THRESH_NORMALS_ARE_PARALLEL		(0.999845f)	
#define THRESH_NORMALS_ARE_ORTHOGONAL	(0.017455f)	

#define THRESH_VECTOR_NORMALIZED		(0.01f)		
#define THRESH_QUAT_NORMALIZED			(0.01f)		

struct MMath
{
public:

	static void SRandInit(int32 seed);

	static int32 GetRandSeed();

	static float SRand();

	static float Atan2(float y, float x);

	static inline float FloatSelect(float comparand, float valueGEZero, float valueLTZero)
	{
		return comparand >= 0.f ? valueGEZero : valueLTZero;
	}

    static inline float TruncToFloat(float f)
	{
		return (float)TruncToInt(f);
	}

	static inline int32 TruncToInt(float f)
	{
		return (int32)(f);
	}

	static inline int32 FloorToInt(float f)
	{
		return TruncToInt(floorf(f));
	}

	static inline float FloorToFloat(float f)
	{
		return floorf(f);
	}

	static inline double FloorToDouble(double f)
	{
		return floor(f);
	}

	static inline int32 RoundToInt(float f)
	{
		return FloorToInt(f + 0.5f);
	}

	static inline float RoundToFloat(float f)
	{
		return FloorToFloat(f + 0.5f);
	}

	static inline double RoundToDouble(double f)
	{
		return FloorToDouble(f + 0.5);
	}

	static inline int32 CeilToInt(float f)
	{
		return TruncToInt(ceilf(f));
	}

	static inline float CeilToFloat(float f)
	{
		return ceilf(f);
	}

	static inline double CeilToDouble(double f)
	{
		return ceil(f);
	}

	static inline float Fractional(float value)
	{
		return value - TruncToFloat(value);
	}

	static inline float Frac(float value)
	{
		return value - FloorToFloat(value);
	}

	static inline float Modf(const float value, float* outIntPart)
	{
		return modff(value, outIntPart);
	}

	static inline double Modf(const double value, double* outIntPart)
	{
		return modf(value, outIntPart);
	}

	static inline float Exp(float value)
	{ 
		return expf(value);
	}

	static inline float Exp2(float value)
	{ 
		return powf(2.f, value);
	}

	static inline float Loge(float value)
	{ 
		return logf(value);
	}

	static inline float LogX(float base, float value)
	{ 
		return Loge(value) / Loge(base);
	}

	static inline float Log2(float value)
	{ 
		return Loge(value) * 1.4426950f;
	}

	static inline float Fmod(float x, float y)
	{
		if (fabsf(y) <= 1.e-8f) {
			return 0.f;
		}

		const float quotient = TruncToFloat(x / y);
		float intPortion = y * quotient;

		if (fabsf(intPortion) > fabsf(x)) {
			intPortion = x;
		}

		const float result = x - intPortion;
		return result;
	}
	
	static inline float Sin(float value) 
	{ 
		return sinf(value);
	}

	static inline float Asin(float value)
	{ 
		return asinf((value < -1.f) ? -1.f : ((value < 1.f) ? value : 1.f));
	}

	static inline float Sinh(float value)
	{ 
		return sinhf(value);
	}

	static inline float Cos(float value)
	{ 
		return cosf(value);
	}

	static inline float Acos(float value)
	{ 
		return acosf((value < -1.f) ? -1.f : ((value < 1.f) ? value : 1.f));
	}

	static inline float Tan(float value)
	{ 
		return tanf(value);
	}

	static inline float Atan(float value)
	{ 
		return atanf(value);
	}

	static inline float Sqrt(float value)
	{ 
		return sqrtf(value);
	}

	static inline float Pow(float a, float b) 
	{ 
		return powf(a, b);
	}

	static inline float InvSqrt(float f)
	{
		return 1.0f / sqrtf(f);
	}

	static inline float InvSqrtEst(float f)
	{
		return InvSqrt(f);
	}

	static inline bool IsNaN(float f)
	{
		return ((*(uint32*)&f) & 0x7FFFFFFF) > 0x7F800000;
	}

	static inline bool IsFinite(float f)
	{
		return ((*(uint32*)&f) & 0x7F800000) != 0x7F800000;
	}

	static inline bool IsNegativeFloat(const float& f)
	{
		return ((*(uint32*)&f) >= (uint32)0x80000000);
	}

	static inline bool IsNegativeDouble(const double& f)
	{
		return ((*(uint64*)&f) >= (uint64)0x8000000000000000);
	}

	static inline int32 Rand() 
	{ 
		return rand();
	}

	static inline void RandInit(int32 seed) 
	{ 
		srand(seed);
	}

	static inline float FRand() 
	{ 
		return Rand() / (float)RAND_MAX;
	}

	static inline uint32 FloorLog2(uint32 value)
	{
		uint32 pos = 0;

		if (value >= 1 << 16) 
		{ 
			value >>= 16; 
			pos += 16;
		}

		if (value >= 1 << 8) 
		{ 
			value >>= 8; 
			pos += 8;
		}

		if (value >= 1 << 4) 
		{ 
			value >>= 4; 
			pos += 4;
		}

		if (value >= 1 << 2) 
		{ 
			value >>= 2; 
			pos += 2;
		}

		if (value >= 1 << 1) 
		{ 
			pos += 1;
		}

		return (value == 0) ? 0 : pos;
	}

	static inline uint64 FloorLog2_64(uint64 value)
	{
		uint64 pos = 0;
		if (value >= 1ull << 32) 
		{ 
			value >>= 32; 
			pos += 32;
		}

		if (value >= 1ull << 16) 
		{ 
			value >>= 16; 
			pos += 16;
		}

		if (value >= 1ull << 8) 
		{ 
			value >>= 8; 
			pos += 8;
		}

		if (value >= 1ull << 4) 
		{ 
			value >>= 4;
			pos += 4;
		}

		if (value >= 1ull << 2) 
		{ 
			value >>= 2; 
			pos += 2;
		}

		if (value >= 1ull << 1) 
		{ 
			pos += 1;
		}

		return (value == 0) ? 0 : pos;
	}

	static inline uint32 CountLeadingZeros(uint32 value)
	{
		if (value == 0) {
			return 32;
		}
		return 31 - FloorLog2(value);
	}

	static inline uint64 CountLeadingZeros64(uint64 value)
	{
		if (value == 0) {
			return 64;
		}
		return 63 - FloorLog2_64(value);
	}

	static inline uint32 CountTrailingZeros(uint32 value)
	{
		if (value == 0) {
			return 32;
		}

		uint32 result = 0;

		while ((value & 1) == 0) 
		{
			value >>= 1;
			++result;
		}

		return result;
	}

	static inline uint64 CountTrailingZeros64(uint64 value)
	{
		if (value == 0) {
			return 64;
		}

		uint64 result = 0;
		while ((value & 1) == 0) 
		{
			value >>= 1;
			++result;
		}

		return result;
	}

	static inline uint32 CeilLogTwo(uint32 value)
	{
		int32 bitmask = ((int32)(CountLeadingZeros(value) << 26)) >> 31;
		return (32 - CountLeadingZeros(value - 1)) & (~bitmask);
	}

	static inline uint64 CeilLogTwo64(uint64 value)
	{
		int64 bitmask = ((int64)(CountLeadingZeros64(value) << 57)) >> 63;
		return (64 - CountLeadingZeros64(value - 1)) & (~bitmask);
	}

	static inline uint32 RoundUpToPowerOfTwo(uint32 value)
	{
		return 1 << CeilLogTwo(value);
	}

	static inline uint64 RoundUpToPowerOfTwo64(uint64 value)
	{
		return uint64(1) << CeilLogTwo64(value);
	}

	template< class T >
	static inline T Abs(const T a)
	{
		return (a >= (T)0) ? a : -a;
	}

	template< class T >
	static inline T Sign(const T a)
	{
		return (a > (T)0) ? (T)1 : ((a < (T)0) ? (T)-1 : (T)0);
	}

	template< class T >
	static inline T Max(const T a, const T b)
	{
		return (a >= b) ? a : b;
	}

	template< class T >
	static inline T Min(const T a, const T b)
	{
		return (a <= b) ? a : b;
	}

	template< class T >
	static inline T Min(const std::vector<T>& values, int32* minIndex = NULL)
	{
		if (values.size() == 0)
		{
			if (minIndex) {
				*minIndex = -1;
			}
			return T();
		}

		T curMin = values[0];
		int32 curMinIndex = 0;
		for (int32 v = 1; v < values.size(); ++v)
		{
			const T value = values[v];
			if (value < curMin) 
			{
				curMin = value;
				curMinIndex = v;
			}
		}

		if (minIndex) {
			*minIndex = curMinIndex;
		}

		return curMin;
	}

	template< class T >
	static inline T Max(const std::vector<T>& values, int32* maxIndex = NULL)
	{
		if (values.size() == 0)
		{
			if (maxIndex) {
				*maxIndex = -1;
			}
			return T();
		}

		T curMax = values[0];
		int32 curMaxIndex = 0;
		for (int32 v = 1; v < values.size(); ++v)
		{
			const T value = values[v];
			if (curMax < value) 
			{
				curMax = value;
				curMaxIndex = v;
			}
		}

		if (maxIndex) {
			*maxIndex = curMaxIndex;
		}

		return curMax;
	}

	static inline int32 RandHelper(int32 value)
	{
		return value > 0 ? Min(TruncToInt(FRand() * value), value - 1) : 0;
	}

	static inline int32 RandRange(int32 inMin, int32 inMax)
	{
		const int32 range = (inMax - inMin) + 1;
		return inMin + RandHelper(range);
	}

	static inline float RandRange(float inMin, float inMax)
	{
		return FRandRange(inMin, inMax);
	}

	static inline float FRandRange(float inMin, float inMax)
	{
		return inMin + (inMax - inMin) * FRand();
	}

	static inline bool RandBool()
	{
		return (RandRange(0, 1) == 1) ? true : false;
	}

	static inline void Inverse4x4(double* dst, const float* src)
	{
		const double s0 = (double)(src[0]);
		const double s1 = (double)(src[1]);
		const double s2 = (double)(src[2]);
		const double s3 = (double)(src[3]);
		const double s4 = (double)(src[4]);
		const double s5 = (double)(src[5]);
		const double s6 = (double)(src[6]);
		const double s7 = (double)(src[7]);
		const double s8 = (double)(src[8]);
		const double s9 = (double)(src[9]);
		const double s10 = (double)(src[10]);
		const double s11 = (double)(src[11]);
		const double s12 = (double)(src[12]);
		const double s13 = (double)(src[13]);
		const double s14 = (double)(src[14]);
		const double s15 = (double)(src[15]);

		double inv[16];
		inv[0] = s5 * s10 * s15 - s5 * s11 * s14 - s9 * s6 * s15 + s9 * s7 * s14 + s13 * s6 * s11 - s13 * s7 * s10;
		inv[1] = -s1 * s10 * s15 + s1 * s11 * s14 + s9 * s2 * s15 - s9 * s3 * s14 - s13 * s2 * s11 + s13 * s3 * s10;
		inv[2] = s1 * s6  * s15 - s1 * s7  * s14 - s5 * s2 * s15 + s5 * s3 * s14 + s13 * s2 * s7 - s13 * s3 * s6;
		inv[3] = -s1 * s6  * s11 + s1 * s7  * s10 + s5 * s2 * s11 - s5 * s3 * s10 - s9 * s2 * s7 + s9 * s3 * s6;
		inv[4] = -s4 * s10 * s15 + s4 * s11 * s14 + s8 * s6 * s15 - s8 * s7 * s14 - s12 * s6 * s11 + s12 * s7 * s10;
		inv[5] = s0 * s10 * s15 - s0 * s11 * s14 - s8 * s2 * s15 + s8 * s3 * s14 + s12 * s2 * s11 - s12 * s3 * s10;
		inv[6] = -s0 * s6  * s15 + s0 * s7  * s14 + s4 * s2 * s15 - s4 * s3 * s14 - s12 * s2 * s7 + s12 * s3 * s6;
		inv[7] = s0 * s6  * s11 - s0 * s7  * s10 - s4 * s2 * s11 + s4 * s3 * s10 + s8 * s2 * s7 - s8 * s3 * s6;
		inv[8] = s4 * s9  * s15 - s4 * s11 * s13 - s8 * s5 * s15 + s8 * s7 * s13 + s12 * s5 * s11 - s12 * s7 * s9;
		inv[9] = -s0 * s9  * s15 + s0 * s11 * s13 + s8 * s1 * s15 - s8 * s3 * s13 - s12 * s1 * s11 + s12 * s3 * s9;
		inv[10] = s0 * s5  * s15 - s0 * s7  * s13 - s4 * s1 * s15 + s4 * s3 * s13 + s12 * s1 * s7 - s12 * s3 * s5;
		inv[11] = -s0 * s5  * s11 + s0 * s7  * s9 + s4 * s1 * s11 - s4 * s3 * s9 - s8 * s1 * s7 + s8 * s3 * s5;
		inv[12] = -s4 * s9  * s14 + s4 * s10 * s13 + s8 * s5 * s14 - s8 * s6 * s13 - s12 * s5 * s10 + s12 * s6 * s9;
		inv[13] = s0 * s9  * s14 - s0 * s10 * s13 - s8 * s1 * s14 + s8 * s2 * s13 + s12 * s1 * s10 - s12 * s2 * s9;
		inv[14] = -s0 * s5  * s14 + s0 * s6  * s13 + s4 * s1 * s14 - s4 * s2 * s13 - s12 * s1 * s6 + s12 * s2 * s5;
		inv[15] = s0 * s5  * s10 - s0 * s6  * s9 - s4 * s1 * s10 + s4 * s2 * s9 + s8 * s1 * s6 - s8 * s2 * s5;

		double det = s0 * inv[0] + s1 * inv[4] + s2 * inv[8] + s3 * inv[12];
		if (det != 0.0) {
			det = 1.0 / det;
		}

		for (int32 i = 0; i < 16; i++) {
			dst[i] = inv[i] * det;
		}
	}

	static inline bool IsNearlyEqual(float a, float b, float errorTolerance = SMALL_NUMBER)
	{
		return Abs<float>(a - b) <= errorTolerance;
	}

	static inline bool IsNearlyEqual(double a, double b, double errorTolerance = SMALL_NUMBER)
	{
		return Abs<double>(a - b) <= errorTolerance;
	}

	static inline bool IsNearlyZero(float value, float errorTolerance = SMALL_NUMBER)
	{
		return Abs<float>(value) <= errorTolerance;
	}

	static inline bool IsNearlyZero(double value, double errorTolerance = SMALL_NUMBER)
	{
		return Abs<double>(value) <= errorTolerance;
	}

	template <typename T>
	static inline bool IsPowerOfTwo(T value)
	{
		return ((value & (value - 1)) == (T)0);
	}

	template< class T >
	static inline T Max3(const T a, const T b, const T c)
	{
		return Max(Max(a, b), c);
	}

	template< class T >
	static inline T Min3(const T a, const T b, const T c)
	{
		return Min(Min(a, b), c);
	}

	template< class T >
	static inline T Square(const T a)
	{
		return a * a;
	}

	template< class T >
	static inline T Clamp(const T x, const T inMin, const T inMax)
	{
		return x < inMin ? inMin : x < inMax ? x : inMax;
	}

	template <class T>
	static inline T DivideAndRoundUp(T dividend, T divisor)
	{
		return (dividend + divisor - 1) / divisor;
	}

	template <class T>
	static inline T DivideAndRoundDown(T dividend, T divisor)
	{
		return dividend / divisor;
	}

	template <class T>
	static inline T DivideAndRoundNearest(T dividend, T divisor)
	{
		return (dividend >= 0)
			? (dividend + divisor / 2) / divisor
			: (dividend - divisor / 2 + 1) / divisor;
	}
    
	static inline void SinCos(float* scalarSin, float* scalarCos, float value)
	{
		float quotient = (INV_PI * 0.5f) * value;
		if (value >= 0.0f) {
			quotient = (float)((int32)(quotient + 0.5f));
		}
		else {
			quotient = (float)((int32)(quotient - 0.5f));
		}

		float y = value - (2.0f * PI) * quotient;

		float sign;
		if (y > HALF_PI) {
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI) {
			y = -PI - y;
			sign = -1.0f;
		}
		else {
			sign = +1.0f;
		}

		float y2 = y * y;

		*scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*scalarCos = sign * p;
	}

#define FASTASIN_HALF_PI (1.5707963050f)

	static inline float FastAsin(float value)
	{
		bool nonnegative = (value >= 0.0f);
		float x = MMath::Abs(value);
		float omx = 1.0f - x;
		if (omx < 0.0f) {
			omx = 0.0f;
		}
		float root = MMath::Sqrt(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root;
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI

	template<class T>
	static inline auto RadiansToDegrees(T const& radVal) -> decltype(radVal * (180.f / PI))
	{
		return radVal * (180.f / PI);
	}

	template<class T>
	static inline auto DegreesToRadians(T const& degVal) -> decltype(degVal * (PI / 180.f))
	{
		return degVal * (PI / 180.f);
	}
    
    static inline void VectorMatrixMultiply(void* result, const void* matrix1, const void* matrix2)
    {
        typedef float Float4x4[4][4];
        const Float4x4& a = *((const Float4x4*) matrix1);
        const Float4x4& b = *((const Float4x4*) matrix2);
        
        Float4x4 temp;
        temp[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
        temp[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
        temp[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
        temp[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];
        
        temp[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
        temp[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
        temp[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
        temp[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3];
        
        temp[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
        temp[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
        temp[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
        temp[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];
        
        temp[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
        temp[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
        temp[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
        temp[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];
        
        memcpy(result, &temp, 16 * sizeof(float));
    }
    
    static inline void VectorMatrixInverse(void* dstMatrix, const void* srcMatrix)
    {
        typedef float Float4x4[4][4];
        const Float4x4& m = *((const Float4x4*)srcMatrix);
        Float4x4 result;
        float det[4];
        Float4x4 tmp;
        
        tmp[0][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
        tmp[0][1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
        tmp[0][2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
        
        tmp[1][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
        tmp[1][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
        tmp[1][2] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
        
        tmp[2][0] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
        tmp[2][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
        tmp[2][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];
        
        tmp[3][0] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
        tmp[3][1] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
        tmp[3][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];
        
        det[0] = m[1][1] * tmp[0][0] - m[2][1] * tmp[0][1] + m[3][1] * tmp[0][2];
        det[1] = m[0][1] * tmp[1][0] - m[2][1] * tmp[1][1] + m[3][1] * tmp[1][2];
        det[2] = m[0][1] * tmp[2][0] - m[1][1] * tmp[2][1] + m[3][1] * tmp[2][2];
        det[3] = m[0][1] * tmp[3][0] - m[1][1] * tmp[3][1] + m[2][1] * tmp[3][2];
        
        float determinant = m[0][0] * det[0] - m[1][0] * det[1] + m[2][0] * det[2] - m[3][0] * det[3];
        const float rDet = 1.0f / determinant;
        
        result[0][0] =  rDet * det[0];
        result[0][1] = -rDet * det[1];
        result[0][2] =  rDet * det[2];
        result[0][3] = -rDet * det[3];
        result[1][0] = -rDet * (m[1][0]*tmp[0][0] - m[2][0]*tmp[0][1] + m[3][0]*tmp[0][2]);
        result[1][1] =  rDet * (m[0][0]*tmp[1][0] - m[2][0]*tmp[1][1] + m[3][0]*tmp[1][2]);
        result[1][2] = -rDet * (m[0][0]*tmp[2][0] - m[1][0]*tmp[2][1] + m[3][0]*tmp[2][2]);
        result[1][3] =  rDet * (m[0][0]*tmp[3][0] - m[1][0]*tmp[3][1] + m[2][0]*tmp[3][2]);
        result[2][0] =  rDet * (
                                m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
                                m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
                                m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
                                );
        result[2][1] = -rDet * (
                                m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
                                m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
                                m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
                                );
        result[2][2] =  rDet * (
                                m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
                                m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
                                m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
                                );
        result[2][3] = -rDet * (
                                m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
                                m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
                                m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
                                );
        result[3][0] = -rDet * (
                                m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
                                m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
                                m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                                );
        result[3][1] =  rDet * (
                                m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
                                m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
                                m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
                                );
        result[3][2] = -rDet * (
                                m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
                                m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
                                m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
                                );
        result[3][3] =  rDet * (
                                m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
                                m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
                                m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
                                );
        
        memcpy(dstMatrix, &result, 16 * sizeof(float));
    }
    
    static inline void VectorTransformVector(void* result, const void* vec,  const void* matrix)
    {
        typedef float Float4[4];
        typedef float Float4x4[4][4];
        
        const Float4& vec4  = *((const Float4*)vec);
        const Float4x4& m44 = *((const Float4x4*)matrix);
        Float4& rVec4 = *((Float4*)result);
        
        rVec4[0] = vec4[0] * m44[0][0] + vec4[1] * m44[1][0] + vec4[2] * m44[2][0] + vec4[3] * m44[3][0];
        rVec4[1] = vec4[0] * m44[0][1] + vec4[1] * m44[1][1] + vec4[2] * m44[2][1] + vec4[3] * m44[3][1];
        rVec4[2] = vec4[0] * m44[0][2] + vec4[1] * m44[1][2] + vec4[2] * m44[2][2] + vec4[3] * m44[3][2];
        rVec4[3] = vec4[0] * m44[0][3] + vec4[1] * m44[1][3] + vec4[2] * m44[2][3] + vec4[3] * m44[3][3];
    }
    
	static inline void VectorQuaternionMultiply(void* result, const void* quat1, const void* quat2)
	{
		typedef float Float4[4];
		const Float4& a = *((const Float4*)quat1);
		const Float4& b = *((const Float4*)quat2);
		Float4& r = *((Float4*)result);

		const float t0 = (a[2] - a[1]) * (b[1] - b[2]);
		const float t1 = (a[3] + a[0]) * (b[3] + b[0]);
		const float t2 = (a[3] - a[0]) * (b[1] + b[2]);
		const float t3 = (a[1] + a[2]) * (b[3] - b[0]);
		const float t4 = (a[2] - a[0]) * (b[0] - b[1]);
		const float t5 = (a[2] + a[0]) * (b[0] + b[1]);
		const float t6 = (a[3] + a[1]) * (b[3] - b[2]);
		const float t7 = (a[3] - a[1]) * (b[3] + b[2]);
		const float t8 = t5 + t6 + t7;
		const float t9 = 0.5f * (t4 + t8);

		r[0] = t1 + t9 - t8;
		r[1] = t2 + t9 - t7;
		r[2] = t3 + t9 - t6;
		r[3] = t0 + t9 - t5;
	}

	template< class T, class U >
	static inline T Lerp(const T& a, const T& b, const U& alpha)
	{
		return (T)(a + alpha * (b - a));
	}

	static inline float RoundFromZero(float f)
	{
		return (f < 0.0f) ? FloorToFloat(f) : CeilToFloat(f);
	}

	static inline double RoundFromZero(double f)
	{
		return (f < 0.0) ? FloorToDouble(f) : CeilToDouble(f);
	}

	static inline float RoundToZero(float f)
	{
		return (f < 0.0f) ? CeilToFloat(f) : FloorToFloat(f);
	}

	static inline double RoundToZero(double f)
	{
		return (f < 0.0) ? CeilToDouble(f) : FloorToDouble(f);
	}

	static inline float RoundToNegativeInfinity(float f)
	{
		return FloorToFloat(f);
	}

	static inline double RoundToNegativeInfinity(double f)
	{
		return FloorToDouble(f);
	}

	static inline float RoundToPositiveInfinity(float f)
	{
		return CeilToFloat(f);
	}

	static inline double RoundToPositiveInfinity(double f)
	{
		return CeilToDouble(f);
	}

	static inline float SmoothStep(float a, float b, float X)
	{
		if (X < a) {
			return 0.0f;
		}
		else if (X >= b) {
			return 1.0f;
		}
		const float InterpFraction = (X - a) / (b - a);
		return InterpFraction * InterpFraction * (3.0f - 2.0f * InterpFraction);
	}

	static Vector2 RandPointInCircle(float circleRadius);

	static float GetRangeValue(Vector2 const& range, float pct);

	static float InterpTo(float current, float target, float deltaTime, float interpSpeed);
};
