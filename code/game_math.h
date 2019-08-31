
#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>
#include "common.h"

#define EPSILON 0.001f
#define PI 3.1415926535897932f
#define TAU 6.2831853071795865f
#define PIO2 1.5707963267948966f
#define PIO4 0.7853981633974483f
#define FOPI 1.27323954473516f

#define _HUGE_ENUF  1e+300
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))

#define DP1 0.78515625f
#define DP2 2.4187564849853515625e-4f
#define DP3 3.77489497744594108e-8f

#define SINCOF0 -1.9515295891E-4f
#define SINCOF1 8.3321608736E-3f
#define SINCOF2 -1.6666654611E-1f

#define COSCOF0 2.443315711809948E-005f
#define COSCOF1 -1.388731625493765E-003f
#define COSCOF2 4.166664568298827E-002f

#define RadToDeg 57.2957795131f
#define DegToRad 0.0174532925199f

#define v3up 	   V3( 0.0f,  1.0f,  0.0f)
#define v3down 	 V3( 0.0f, -1.0f,  0.0f)
#define v3forward   V3( 0.0f,  0.0f,  1.0f)
#define v3back 	 V3( 0.0f,  0.0f, -1.0f)
#define v3left 	 V3(-1.0f,  0.0f,  0.0f)
#define v3right 	V3( 1.0f,  0.0f,  0.0f)
#define v3zero	  V3( 0.0f,  0.0f,  0.0f)


#define Sign(x) ((x) >= 0 ? 1 : -1)

struct aabb {
	v3 min;
	v3 max;

	v3 radiance[6];
	f32 weight[6];
	v3 emission;
};


inline i32 Abs(i32 value) { return value >= 0 ? value : -value; }

// f32 math

//#define Min(a, b) ( (a <= b) ? a : b )
//#define Max(a, b) ( (a >= b) ? a : b )

inline f32 Min(f32 a, f32 b) { return _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(a), _mm_set_ss(b))); }
inline f32 Max(f32 a, f32 b) { return _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(a), _mm_set_ss(b))); }
inline f32 Clamp(f32 value, f32 min, f32 max) {return Min(max, Max(min, value)); }
inline f32 Clamp01(f32 value) { return Clamp(value, 0.0f, 1.0f); }

inline f32 Lerp(f32 a, f32 b, f32 t) { return a * (1.0f - t) + b * t; }

inline f32 Sqrt(f32 value) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value))); }
inline f32 RSqrt(f32 value) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(value))); }

static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_cvtsi32_si128(0x80000000));
inline f32 Abs(f32 value) {
    return _mm_cvtss_f32(_mm_andnot_ps(SIGNMASK, _mm_set_ss(value)));
}

inline f32 Round(f32 value) { return (f32)_mm_cvtss_si32(_mm_set_ss(value)); }
inline f32 Floor(f32 value) { return Round(value - 0.4999f); }
inline f32 Ceil(f32 value) { return Round(value + 0.4999f); }
inline f32 Wrap(f32 value, f32 min, f32 max) {
	f32 range = max - min;
	return value - Floor((value - min) / range) * range;
}
inline f32 Mod(f32 value, f32 divisor) { 
	divisor = Abs(divisor);
	return value - divisor * (i32)(value / divisor);
}

inline f32 Sin(f32 value)
{
	i32 sign = Sign(value);
	f32 x = Abs(value);
	i32 j = (i32)(x * FOPI);
	f32 y = (f32)j;
	
	if (j & 1)
	{
		j += 1;
		y += 1.0f;
	}
	
	j &= 7;
	
	if (j > 3)
	{
		sign = -sign;
		j -= 4;
	}
	
	x = ((x - y * DP1) - y * DP2) - y * DP3;
	f32 z = x * x;
	
	if (j == 1 || j == 2)
	{
		y = COSCOF0;
		y = y * z + COSCOF1;
		y = y * z + COSCOF2;
		y *= z * z;
		y -= 0.5f * z;
		y += 1.0f;
	}
	else
	{
		y = SINCOF0;
		y = y * z + SINCOF1;
		y = y * z + SINCOF2;
		y *= z * x;
		y += x;
	}
	
	return y * sign;
}

inline f32 Cos(f32 value)
{
	i32 sign = 1;
	f32 x = Abs(value);
	i32 j = (i32)(x * FOPI);
	f32 y = (f32)j;
	
	if (j & 1)
	{
		j += 1;
		y += 1.0f;
	}
	
	j &= 7;
	
	if (j > 3)
	{
		sign = -sign;
		j -= 4;
	}
	
	if (j > 1)
		sign = -sign;
	
	x = ((x - y * DP1) - y * DP2) - y * DP3;
	f32 z = x * x;
	
	if (j == 1 || j == 2)
	{
		y = SINCOF0;
		y = y * z + SINCOF1;
		y = y * z + SINCOF2;
		y *= z * x;
		y += x;
	}
	else
	{
		y = COSCOF0;
		y = y * z + COSCOF1;
		y = y * z + COSCOF2;
		y *= z * z;
		y -= 0.5f * z;
		y += 1.0f;
	}
	
	return y * sign;
}

inline f32 Tan(f32 value)
{
	i32 sign = Sign(value);
	f32 x = Abs(value);
	
	i32 j = (i32)(x * FOPI);
	f32 y = (f32)j;
	
	if (j & 1)
	{
		j += 1;
		y += 1.0f;
	}
	
	f32 z = ((x - y * DP1) - y * DP2) - y * DP3;
	f32 zz = z * z;
	
	if (x > 1.0e-4f)
	{
		y =
			((((( 9.38540185543E-3f * zz
				 + 3.11992232697E-3f) * zz
				+ 2.44301354525E-2f) * zz
			   + 5.34112807005E-2f) * zz
			  + 1.33387994085E-1f) * zz
			 + 3.33331568548E-1f) * zz * z
			+ z;
	}
	else
	{
		y = z;
	}
	
	if (j & 2)
	{
		y = -1.0f / y;
	}
	
	return y * sign;
}

inline f32 Asin(f32 value)
{
	i32 sign = Sign(value);
	f32 x = value;
	f32 a = Abs(value);
	i32 flag;
	f32 z;
	
	Assert(a <= 1.0f);
	
	if (a < 1.0e-4f)
	{
		return x;
	}
	
	if (a > 0.5f)
	{
		z = 0.5f * (1.0f - a);
		x = Sqrt(z);
		flag = 1;
	}
	else
	{
		x = a;
		z = x * x;
		flag = 0;
	}
	
	z =
		(((( 4.2163199048E-2f * z
			+ 2.4181311049E-2f) * z
		   + 4.5470025998E-2f) * z
		  + 7.4953002686E-2f) * z
		 + 1.6666752422E-1f) * z * x
		+ x;
	
	if (flag != 0)
	{
		z = z + z;
		z = PIO2 - z;
	}
	
	return z * sign;
}

inline f32 Acos(f32 value)
{
	Assert(value <= 1.0f);
	Assert(value >= -1.0f);
	
	if (value < -0.5f)
		return PI - 2.0f * Asin(Sqrt(0.5f * (1.0f + value)));
	
	if (value > 0.5f)
		return 2.0f * Asin(Sqrt(0.5f * (1.0f - value)));
	
	return PIO2 - Asin(value);
}

inline f32 Atan(f32 value)
{
	i32 sign = Sign(value);
	f32 x = Abs(value);
	f32 y = 0.0f;
	
	if (x > 2.414213562373095f)
	{
		y = PIO2;
		x = -(1.0f / x);
	}
	else if (x > 0.4142135623730950f)
	{
		y = PIO4;
		x = (x - 1.0f) / (x + 1.0f);
	}
	
	f32 z = x * x;
	y += ((( 8.05374449538e-2f * z
		- 1.38776856032E-1f) * z
		+ 1.99777106478E-1f) * z
		- 3.33329491539E-1f) * z * x
		+ x;
		
	return y * sign;
}

//inline f32 Atan2(f32 x, f32 y) { return 1.0f; }

// V2 math

inline v2 V2(f32 x, f32 y) { v2 result = {x, y}; return result; }
inline v2 V2(i32 x, i32 y) { v2 result = {(f32)x, (f32)y}; return result; }
inline v2 V2(u32 x, u32 y) { v2 result = {(f32)x, (f32)y}; return result; }

inline bool operator==(v2 a, v2 b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(v2 a, v2 b) { return !operator==(a, b); }

inline v2 operator-(v2 a) {a.x = -a.x; a.y = -a.y; return a; }

inline v2 operator+(v2 a, v2 b) { a.x += b.x; a.y += b.y; return a; }
inline v2 operator+(v2 v, f32 value) { v.x += value; v.y += value; return v; }
inline v2 operator+(f32 value, v2 v) { v.x += value; v.y += value; return v; }

inline v2 operator-(v2 a, v2 b) { a.x -= b.x; a.y -= b.y; return a; }
inline v2 operator-(v2 v, f32 value) { v.x -= value; v.y -= value; return v; }
inline v2 operator-(f32 value, v2 v) { v.x = value - v.x; v.y = value - v.y; return v; }

inline v2 operator*(v2 a, v2 b)  { a.x *= b.x; a.y *= b.y; return a; }
inline v2 operator*(v2 v, f32 value) { v.x *= value; v.y *= value; return v; }
inline v2 operator*(f32 value, v2 v) { v.x *= value; v.y *= value; return v; }

inline v2 operator/(v2 a, v2 b)  { a.x /= b.x; a.y /= b.y; return a; }
inline v2 operator/(v2 v, f32 value) { return operator*(v, 1.0f / value); }
inline v2 operator/(f32 value, v2 v) { v.x = value / v.x; v.y = value / v.y; return v; }

inline v2 &operator+=(v2 &a, v2 b) { a = a + b; return a; }
inline v2 &operator+=(v2 &v, f32 value) { v = v + value; return v; }
inline v2 &operator-=(v2 &a, v2 b) { a = a - b; return a; }
inline v2 &operator-=(v2 &v, f32 value) { v = v - value; return v; }
inline v2 &operator*=(v2 &a, v2 b) { a = a * b; return a; }
inline v2 &operator*=(v2 &v, f32 value) { v = v * value; return v; }
inline v2 &operator/=(v2 &a, v2 b) { a = a / b; return a; }
inline v2 &operator/=(v2 &v, f32 value) { v = v / value; return v; }

inline f32 Dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
inline f32 Cross(v2 a, v2 b) { return a.x * b.y - b.x * a.y; }

inline f32 Length2(v2 a) { return Dot(a, a); }
inline f32 Length(v2 a) { return Sqrt(Length2(a)); }

inline v2 Normalize(v2 a) { return a * (1.0f / Length(a)); }
inline v2 MaxLength1(v2 a) { f32 l = Length(a); if (l > 1.0f) a = a * (1.0f / l); return a; }

inline v2 Lerp(v2 a, v2 b, f32 t) { return a * (1.0f - t) + b * t; }

// Vector2 integer math

inline v2i V2I(i32 x, i32 y) { v2i result = {x, y}; return result; }

inline bool operator==(v2i a, v2i b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(v2i a, v2i b) { return !operator==(a, b); }

inline v2i operator-(v2i a) {a.x = -a.x; a.y = -a.y; return a; }

inline v2i operator+(v2i a, v2i b) { a.x += b.x; a.y += b.y; return a; }
inline v2i operator+(v2i v, i32 value) { v.x += value; v.y += value; return v; }
inline v2i operator+(i32 value, v2i v) { v.x += value; v.y += value; return v; }

inline v2i operator-(v2i a, v2i b) { a.x -= b.x; a.y -= b.y; return a; }
inline v2i operator-(v2i v, i32 value) { v.x -= value; v.y -= value; return v; }
inline v2i operator-(i32 value, v2i v) { v.x = value - v.x; v.y = value - v.y; return v; }

inline v2i operator*(v2i a, v2i b)  { a.x *= b.x; a.y *= b.y; return a; }
inline v2i operator*(v2i v, i32 value) { v.x *= value; v.y *= value; return v; }
inline v2i operator*(i32 value, v2i v) { v.x *= value; v.y *= value; return v; }

inline v2i operator/(v2i a, v2i b)  { a.x /= b.x; a.y /= b.y; return a; }
inline v2i operator/(v2i v, i32 value) { return operator*(v, 1 / value); }
inline v2i operator/(i32 value, v2i v) { v.x = value / v.x; v.y = value / v.y; return v; }

inline v2i &operator+=(v2i &a, v2i b) { a = a + b; return a; }
inline v2i &operator+=(v2i &v, i32 value) { v = v + value; return v; }
inline v2i &operator-=(v2i &a, v2i b) { a = a - b; return a; }
inline v2i &operator-=(v2i &v, i32 value) { v = v - value; return v; }
inline v2i &operator*=(v2i &a, v2i b) { a = a * b; return a; }
inline v2i &operator*=(v2i &v, i32 value) { v = v * value; return v; }
inline v2i &operator/=(v2i &a, v2i b) { a = a / b; return a; }
inline v2i &operator/=(v2i &v, i32 value) { v = v / value; return v; }

// Vector3 math

inline v3 V3(f32 x, f32 y, f32 z) { v3 result = {x, y, z}; return result; }
inline v3 V3(i32 x, i32 y, i32 z) { v3 result = {(f32)x, (f32)y, (f32)z}; return result; }
inline v3 V3(u32 x, u32 y, u32 z) { v3 result = {(f32)x, (f32)y, (f32)z}; return result; }

inline v3 V3(v2 a, f32 z) { v3 result = {a.x, a.y, z}; return result; }

inline bool operator==(v3 a, v3 b) { return Abs(a.x - b.x) < 0.001f && Abs(a.y - b.y) < 0.001f && Abs(a.z - b.z) < 0.001f; }
inline bool operator!=(v3 a, v3 b) { return !operator==(a, b); }

inline v3 operator-(v3 v) { v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }

inline v3 operator+(v3 a, v3 b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
inline v3 operator+(v3 v, f32 value) { v.x += value; v.y += value; v.z += value; return v; }
inline v3 operator+(f32 value, v3 v) { v.x += value; v.y += value; v.z += value; return v; }

inline v3 operator-(v3 a, v3 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
inline v3 operator-(v3 v, f32 value) { v.x -= value; v.y -= value; v.z -= value; return v; }
inline v3 operator-(f32 value, v3 v) { v.x = value - v.x; v.y = value - v.y; v.z = value - v.z; return v; }

inline v3 operator*(v3 a, v3 b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return a; }
inline v3 operator*(v3 v, f32 value) { v.x *= value; v.y *= value; v.z *= value; return v; }
inline v3 operator*(f32 value, v3 v) { v.x *= value; v.y *= value; v.z *= value; return v; }

inline v3 operator/(v3 a, v3 b) { a.x /= b.x; a.y /= b.y; a.z /= b.z; return a; }
inline v3 operator/(v3 v, f32 value) { return operator*(v, 1.0f / value); }
inline v3 operator/(f32 value, v3 v) { v.x = value / v.x; v.y = value / v.y; v.z = value / v.z; return v; }

inline v3 &operator+=(v3 &a, v3 b) { a = a + b; return a; }
inline v3 &operator+=(v3 &v, f32 value) { v = v + value; return v; }
inline v3 &operator-=(v3 &a, v3 b) { a = a - b; return a; }
inline v3 &operator-=(v3 &v, f32 value) { v = v - value; return v; }
inline v3 &operator*=(v3 &a, v3 b) { a = a * b; return a; }
inline v3 &operator*=(v3 &v, f32 value) { v = v * value; return v; }
inline v3 &operator/=(v3 &a, v3 b) { a = a / b; return a; }
inline v3 &operator/=(v3 &v, f32 value) { v = v / value; return v; }

inline f32 Dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline v3 Cross(v3 a, v3 b) { v3 result = {}; result.x = a.y*b.z - a.z*b.y; result.y = a.z*b.x - a.x*b.z; result.z = a.x*b.y - a.y*b.x; return result; }

inline f32 Length2(v3 a) { return Dot(a, a); }
inline f32 Length(v3 a) { return Sqrt(Length2(a)); }

inline v3 Normalize(v3 a) { return a * (1.0f / Length(a)); }

inline v3 Lerp(v3 a, v3 b, f32 t) { return a * (1.0f - t) + b * t; }

inline v3 OOD(v3 v)
{
	v3 result = {INFINITY, INFINITY, INFINITY};
	
	if (Abs(v.x) > EPSILON)
		result.x = 1.0f / v.x;
	if (Abs(v.y) > EPSILON)
		result.y = 1.0f / v.y;
	if (Abs(v.z) > EPSILON)
		result.z = 1.0f / v.z;
	
	return result;
}

inline v3 MoveTowards(v3 a, v3 b, f32 maxDistance)
{
	/*Vector3 vector3 = target - current;
	  float magnitude = vector3.magnitude;
	  if ((double) magnitude <= (double) maxDistanceDelta || (double) magnitude == 0.0)
		return target;
	  return current + vector3 / magnitude * maxDistanceDelta;*/
	v3 diff = b - a;
	f32 l = Length(diff);
	
	if (l < maxDistance || l < EPSILON)
		return b;
	
	return a + diff / l * maxDistance;
}

inline f32 AngleX(v3 direction)
{
	direction.x = 0.0f;
	direction = Normalize(direction);
	
	if (direction.y >= 0)
	{
		return -Acos(direction.z);
	}
	else
	{
		return Acos(direction.z);
	}
}

inline f32 AngleY(v3 direction)
{
	direction.y = 0.0f;
	direction = Normalize(direction);
	
	if (direction.x <= 0)
	{
		return -Acos(direction.z);
	}
	else
	{
		return Acos(direction.z);
	}
}

inline f32 AngleZ(v3 direction)
{
	direction.z = 0.0f;
	direction = Normalize(direction);
	
	if (direction.y >= 0)
	{
		return -Acos(direction.x);
	}
	else
	{
		return Acos(direction.x);
	}
}

inline v3 V3RotateX(f32 angle)
{
	v3 result = {};
	result.y = -Sin(angle);
	result.z = Cos(angle);
	return result;
}

inline v3 V3RotateX(f32 angle, f32 y, f32 z)
{
	v3 result = {};
	result.y = y * Cos(angle) - z * Sin(angle);
	result.z = z * Cos(angle) + y * Sin(angle);
	return result;
}

inline v3 V3RotateY(f32 angle)
{
	v3 result = {};
	result.x = Sin(angle);
	result.z = Cos(angle);
	return result;
}

inline v3 V3RotateY(f32 angle, f32 x, f32 z)
{
	v3 result = {};
	result.x = z * Sin(angle) + x * Cos(angle);
	result.z = z * Cos(angle) - x * Sin(angle);
	return result;
}

inline v3 V3RotateZ(f32 angle)
{
	v3 result = {};
	result.x = -Sin(angle);
	result.y = Cos(angle);
	return result;
}

inline v3 V3RotateZ(f32 angle, f32 x, f32 y)
{
	v3 result = {};
	result.x = x * -Sin(angle) + y * Cos(angle);
	result.y = x * Cos(angle) + y * Sin(angle);
	return result;
}

// v4 math

inline v4 V4(f32 x, f32 y, f32 z, f32 w) { v4 result = {x, y, z, w}; return result; }
inline v4 V4(v3 a, f32 w) { v4 result = {a.x, a.y, a.z, w}; return result; }

// AABB math

inline aabb AABB(v3 a, v3 b) { aabb result = {a, b}; return result; }
inline aabb AABB(f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) { aabb result = {{x, y, z}, {u, v, w}}; return result; }


// Quaternion math

/*inline quat QuatIdentity() { quat result = {0.0f, 0.0f, 0.0f, 1.0f}; return result; }
inline quat Quat(f32 x, f32 y, f32 z, f32 w) { quat result = {x, y, z, w}; return result; }
inline quat Quat(v2 v, f32 z, f32 w) { quat result = {v.x, v.y, z, w}; return result; }
inline quat Quat(v3 v, f32 w) { quat result = {v.x, v.y, v.z, w}; return result; }
inline quat Quat(v4 v) { quat result = {v.x, v.y, v.z, v.w}; return result; }

inline bool operator==(quat a, quat b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
inline bool operator!=(quat a, quat b) { return !operator==(a, b); }

inline quat operator-(quat q) { q.x = -q.x; q.y = -q.y; q.z = -q.z; q.w = -q.w; return q; }

inline quat operator+(quat a, quat b) { a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; return a; }

inline quat operator*(quat a, quat b)
{
	quat result = {};
	
	result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;;
	
	return result;
}
inline v3 operator*(quat q, v3 v)
{
	v3 u = {q.x, q.y, q.z};
	v3 t = Cross(u, v) * 2.0f;
	
	v3 p = Cross(u, t);
	
	v3 result = t * q.w;
	result += v;
	result += p;
	
	return result;
}
inline quat operator*(quat q, f32 value) { q.x *= value; q.y *= value; q.z *= value; q.w *= value; return q; }
inline quat operator*(f32 value, quat q) { q.x *= value; q.y *= value; q.z *= value; q.w *= value; return q; }

inline quat operator/(quat q, f32 value) { q.x /= value; q.y /= value; q.z /= value; q.w /= value; return q; }
inline quat operator/(f32 value, quat q) { q.x /= value; q.y /= value; q.z /= value; q.w /= value; return q; }

inline quat &operator*=(quat &a, quat b) { a = a * b; return a; }
inline quat &operator*=(quat &q, f32 value) { q = q * value; return q; }
inline quat &operator/=(quat &q, f32 value) { q = q / value; return q; }

inline f32 Dot(quat a, quat b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

inline f32 Length2(quat q) { return Dot(q, q); }
inline f32 Length(quat q) { return Sqrt(Length2(q)); }

inline quat Normalize(quat q) { return q * (1.0f / Length(q)); }

inline quat Lerp(quat a, quat b, f32 t)
{
	f32 d = Dot(a, b);
	if (d < 0.0f)
		b = -b;
	return Quat((b.x - a.x) * t + a.x, (b.y - a.y) * t + a.y, (b.z - a.z) * t + a.z, (b.w - a.w) * t + a.w);
}
inline quat LerpN(quat a, quat b, f32 t) { return Normalize(a * (1.0f - t) + b * t); }

inline quat QuatRotateAround(f32 angle, v3 v)
{
	quat result = {};
	f32 s = Sin(angle * 0.5f);
	
	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	result.w = Cos(angle * 0.5f);
	
	return result;
}

inline quat QuatRotateX(f32 angle)
{
	quat result = {};
	result.x = Sin(angle * 0.5f);
	result.w = Cos(angle * 0.5f);
	return result;
}

inline quat QuatRotateY(f32 angle)
{
	quat result = {};
	result.y = Sin(angle * 0.5f);
	result.w = Cos(angle * 0.5f);
	return result;
}

inline quat QuatRotateZ(f32 angle)
{
	quat result = {};
	result.z = Sin(angle * 0.5f);
	result.w = Cos(angle * 0.5f);
	return result;
}

inline f32 Angle(quat a, quat b)
{
	return Acos(Min(Abs(Dot(a, b)), 1.0f)) * 2.0f;
}

inline quat RotateTowards(quat a, quat b, f32 maxAngle)
{
	f32 t = Min(1.0f, maxAngle / Dot(a, b));
	return Lerp(a, b, t);
}

inline quat LookRotation(v3 forward, v3 up, v3 right)
{
	quat q;
	f32 trace = right.x + up.y + forward.z;
	
	if (trace > 0.0f)
	{
		f32 s = 0.5f / Sqrt(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (up.z - forward.y) * s;
		q.y = (forward.x - right.z) * s;
		q.z = (right.y - up.x) * s;
	}
	else
	{
		if (right.x > up.y && right.x > forward.z)
		{
			f32 s = 2.0f * Sqrt(1.0f + right.x - up.y - forward.z);
			q.w = (up.z - forward.y) / s;
			q.x = 0.25f * s;
			q.y = (up.x + right.y) / s;
			q.z = (forward.x + right.z) / s;
		}
		else if (up.y > forward.z)
		{
			f32 s = 2.0f * Sqrt(1.0f + up.y - right.x - forward.z);
			q.w = (forward.x - right.z) / s;
			q.x = (up.x + right.y) / s;
			q.y = 0.25f * s;
			q.z = (forward.y + up.z) / s;
		}
		else
		{
			f32 s = 2.0f * Sqrt(1.0f + forward.z - right.x - up.y);
			q.w = (right.y - up.x) / s;
			q.x = (forward.x + right.z) / s;
			q.y = (forward.y + up.z) / s;
			q.z = 0.25f * s;
		}
	}
	
	return q;
}

inline quat QuatConj(quat q)
{
	q.x = -q.x;
	q.y = -q.y;
	q.z = -q.z;
	
	return q;
}

inline quat QuatInverse(quat q)
{
	return QuatConj(q) / Dot(q, q);
}*/

// Matrix3 math

/*inline m3 M3() {
	m3 result = {
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		}
	};
	
	return result;
}

inline m3 Matrix3(quat q) {
	q = Normalize(q);
	f32 xx = q.x * q.x;
	f32 yy = q.y * q.y;
	f32 zz = q.z * q.z;
	
	f32 xy = q.x * q.y;
	f32 xz = q.x * q.z;
	f32 yz = q.y * q.z;
	
	f32 wx = q.w * q.x;
	f32 wy = q.w * q.y;
	f32 wz = q.w * q.z;
	
	m3 result;
	
	result.e[0][0] = 1.0f - 2.0f * (yy + zz);
	result.e[1][0] =        2.0f * (xy + wz);
	result.e[2][0] =        2.0f * (xz - wy);
	
	result.e[0][1] =        2.0f * (xy - wz);
	result.e[1][1] = 1.0f - 2.0f * (xx + zz);
	result.e[2][1] =        2.0f * (yz + wx);
	
	result.e[0][2] =        2.0f * (xz + wy);
	result.e[1][2] =        2.0f * (yz - wx);
	result.e[2][2] = 1.0f - 2.0f * (xx + yy);
	
	return result;
}

inline m3 operator*(m3 a, m3 b) {
	m3 result = {};
	
	for(int r = 0; r < 3; r++) {
		for(int c = 0; c < 3; c++) {
			for(int i = 0; i < 3; i++) {
				result.e[r][c] += a.e[i][c]*b.e[r][i];
			}
		}
	}
	
	return result;
}

inline v3 operator*(m3 m, v3 v)
{
	v3 result = {};
	
	result.x = m.e[0][0]*v.x + m.e[0][1]*v.y + m.e[0][2]*v.z;
	result.y = m.e[1][0]*v.x + m.e[1][1]*v.y + m.e[1][2]*v.z;
	result.z = m.e[2][0]*v.x + m.e[2][1]*v.y + m.e[2][2]*v.z;
	
	return result;
}

inline m3 Transpose(m3 m)
{
	f32 tmp;
	tmp = m.e[1][0]; m.e[1][0] = m.e[0][1]; m.e[0][1] = tmp;
	tmp = m.e[2][0]; m.e[2][0] = m.e[0][2]; m.e[0][2] = tmp;
	tmp = m.e[2][1]; m.e[2][1] = m.e[1][2]; m.e[1][2] = tmp;
	
	return m;
}

inline f32 Determinate(m3 m)
{
	return m.e[0][0] * (m.e[1][1] * m.e[2][2] - m.e[2][1] * m.e[1][2])
		-m.e[1][0] * (m.e[0][1] * m.e[2][2] - m.e[2][1] * m.e[0][2])
		+m.e[2][0] * (m.e[0][1] * m.e[1][2] - m.e[1][1] * m.e[0][2]);
}

inline quat ExtractRotation(m3 m, quat q)
{
	v3 mc1 = V3(m.e[0][0], m.e[1][0], m.e[2][0]);
	v3 mc2 = V3(m.e[0][1], m.e[1][1], m.e[2][1]);
	v3 mc3 = V3(m.e[0][2], m.e[1][2], m.e[2][2]);
	
	for (i32 i = 0; i < 10; i++)
	{
		m3 r = Matrix3(q);
		
		v3 rc1 = V3(r.e[0][0], r.e[1][0], r.e[2][0]);
		v3 rc2 = V3(r.e[0][1], r.e[1][1], r.e[2][1]);
		v3 rc3 = V3(r.e[0][2], r.e[1][2], r.e[2][2]);
		
		v3 omega =  (Cross(rc1, mc1) + Cross(rc2, mc2) + Cross(rc3, mc3))
			* (1.0f / Abs(Dot(rc1, mc1) + Dot(rc2, mc2) + Dot(rc3, mc3)) + 1.0e-9f);
		
		f32 w = Length(omega);
		if (w < 1.0e-9f)
			break;
		
		q = Normalize(QuatRotateAround(w, (1.0f / w) * omega) * q);
	}
	
	return q;
}*/

// Matrix4 math

inline m4 M4Identity() {
	m4 result = {
		{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1},
		}
	};
	
	return result;
}

inline m4 M4(f32 x, f32 y, f32 z) {
	m4 result = M4Identity();
	result.e[0][3] = x;
	result.e[1][3] = y;
	result.e[2][3] = z;
	
	return result;
}

inline m4 M4(v3 v) {
	m4 result = M4Identity();
	
	result.e[0][3] = v.x;
	result.e[1][3] = v.y;
	result.e[2][3] = v.z;
	
	return result;
}

/*inline m4 M4(Quat q) {
	f32 xx = q.x * q.x;
	f32 yy = q.y * q.y;
	f32 zz = q.z * q.z;
	
	f32 xy = q.x * q.y;
	f32 xz = q.x * q.z;
	f32 yz = q.y * q.z;
	
	f32 wx = q.w * q.x;
	f32 wy = q.w * q.y;
	f32 wz = q.w * q.z;
	
	m4 result = M4Identity();
	
	result.e[0][0] = 1.0f - 2.0f * (yy + zz);
	result.e[1][0] =        2.0f * (xy + wz);
	result.e[2][0] =        2.0f * (xz - wy);
	
	result.e[0][1] =        2.0f * (xy - wz);
	result.e[1][1] = 1.0f - 2.0f * (xx + zz);
	result.e[2][1] =        2.0f * (yz + wx);
	
	result.e[0][2] =        2.0f * (xz + wy);
	result.e[1][2] =        2.0f * (yz - wx);
	result.e[2][2] = 1.0f - 2.0f * (xx + yy);
	
	return result;
}*/

inline m4 operator+(m4 a, m4 b) {
	m4 result = {};
	
	for (i32 j = 0; j < 4; j++) {
		for (i32 i = 0; i < 4; i++)
			result.e[j][i] = a.e[j][i] + b.e[j][i];
	}
	
	return result;
}

inline m4 operator*(m4 a, m4 b) {
	m4 result = {};
	
	for(int r = 0; r < 4; r++) {
		for(int c = 0; c < 4; c++) {
			for(int i = 0; i < 4; i++) {
				result.e[r][c] += a.e[i][c]*b.e[r][i];
			}
		}
	}
	
	return result;
}

inline v3 operator*(m4 m, v3 v)
{
	v3 result = {};
	
	result.x = m.e[0][0]*v.x + m.e[0][1]*v.y + m.e[0][2]*v.z;
	result.y = m.e[1][0]*v.x + m.e[1][1]*v.y + m.e[1][2]*v.z;
	result.z = m.e[2][0]*v.x + m.e[2][1]*v.y + m.e[2][2]*v.z;
	
	return result;
}

inline v4 operator*(m4 m, v4 v)
{
	v4 result = {};
	
	result.x = m.e[0][0]*v.x + m.e[0][1]*v.y + m.e[0][2]*v.z + m.e[0][3]*v.w;
	result.y = m.e[1][0]*v.x + m.e[1][1]*v.y + m.e[1][2]*v.z + m.e[1][3]*v.w;
	result.z = m.e[2][0]*v.x + m.e[2][1]*v.y + m.e[2][2]*v.z + m.e[2][3]*v.w;
	result.w = m.e[3][0]*v.x + m.e[3][1]*v.y + m.e[3][2]*v.z + m.e[3][3]*v.w;
	
	return result;
}

inline m4 M4Scale(f32 a, f32 b, f32 c) {
	m4 result = M4Identity();
	
	result.e[0][0] = a;
	result.e[1][1] = b;
	result.e[2][2] = c;
	
	return result;
}

inline m4 M4Scale(v3 v) {
	m4 result = M4Identity();
	
	result.e[0][0] = v.x;
	result.e[1][1] = v.y;
	result.e[2][2] = v.z;
	
	return result;
}

inline m4 M4RotateAround(f32 angle, v3 axis)
{
	f32 c = Cos(angle);
	f32 s = Sin(angle);
	
	m4 result = M4Identity();
	v3 t = axis * (1.0f - c);
	
	result.e[0][0] = c + t.x*axis.x;
	result.e[1][0] = 0 + t.x*axis.y + s*axis.z;
	result.e[2][0] = 0 + t.x*axis.z - s*axis.y;
	result.e[3][0] = 0;
	
	result.e[0][1] = 0 + t.y*axis.x - s*axis.z;
	result.e[1][1] = c + t.y*axis.y;
	result.e[2][1] = 0 + t.y*axis.z + s*axis.x;
	result.e[3][1] = 0;
	
	result.e[0][2] = 0 + t.z*axis.x + s*axis.y;
	result.e[1][2] = 0 + t.z*axis.y - s*axis.x;
	result.e[2][2] = c + t.z*axis.z;
	result.e[3][2] = 0;
	
	return result;
}

inline m4 M4RotX(f32 angle) {
	f32 c = Cos(angle);
	f32 s = Sin(angle);
	
	m4 result = M4Identity();
	result.e[1][1] = c;
	result.e[1][2] = -s;
	result.e[2][1] = s;
	result.e[2][2] = c;
	
	return result;
}

inline m4 M4RotY(f32 angle) {
	f32 c = Cos(angle);
	f32 s = Sin(angle);
	
	m4 result = M4Identity();
	result.e[0][0] = c;
	result.e[0][2] = s;
	result.e[2][0] = -s;
	result.e[2][2] = c;
	
	return result;
}

inline m4 M4RotZ(f32 angle) {
	f32 c = Cos(angle);
	f32 s = Sin(angle);
	
	m4 result = M4Identity();
	result.e[0][0] = c;
	result.e[0][1] = -s;
	result.e[1][0] = s;
	result.e[1][1] = c;
	
	return result;
}

inline m4 Transpose(m4 m)
{
	f32 tmp;
	tmp = m.e[1][0]; m.e[1][0] = m.e[0][1]; m.e[0][1] = tmp;
	tmp = m.e[2][0]; m.e[2][0] = m.e[0][2]; m.e[0][2] = tmp;
	tmp = m.e[3][0]; m.e[3][0] = m.e[0][3]; m.e[0][3] = tmp;
	tmp = m.e[2][1]; m.e[2][1] = m.e[1][2]; m.e[1][2] = tmp;
	tmp = m.e[3][1]; m.e[3][1] = m.e[1][3]; m.e[1][3] = tmp;
	tmp = m.e[3][2]; m.e[3][2] = m.e[2][3]; m.e[2][3] = tmp;
	
	return m;
}

inline m4 Inverse(m4 m)
{
	m4 result = {};
	
	f32 sf00 = m.e[2][2] * m.e[3][3] - m.e[2][3] * m.e[3][2];
	f32 sf01 = m.e[1][2] * m.e[3][3] - m.e[1][3] * m.e[3][2];
	f32 sf02 = m.e[1][2] * m.e[2][3] - m.e[1][3] * m.e[2][2];
	f32 sf03 = m.e[0][2] * m.e[3][3] - m.e[0][3] * m.e[3][2];
	f32 sf04 = m.e[0][2] * m.e[2][3] - m.e[0][3] * m.e[2][2];
	f32 sf05 = m.e[0][2] * m.e[1][3] - m.e[0][3] * m.e[1][2];
	f32 sf06 = m.e[2][1] * m.e[3][3] - m.e[2][3] * m.e[3][1];
	f32 sf07 = m.e[1][1] * m.e[3][3] - m.e[1][3] * m.e[3][1];
	f32 sf08 = m.e[1][1] * m.e[2][3] - m.e[1][3] * m.e[2][1];
	f32 sf09 = m.e[0][1] * m.e[3][3] - m.e[0][3] * m.e[3][1];
	f32 sf10 = m.e[0][1] * m.e[2][3] - m.e[0][3] * m.e[2][1];
	f32 sf11 = m.e[1][1] * m.e[3][3] - m.e[1][3] * m.e[3][1];
	f32 sf12 = m.e[0][1] * m.e[1][3] - m.e[0][3] * m.e[1][1];
	f32 sf13 = m.e[2][1] * m.e[3][2] - m.e[2][2] * m.e[3][1];
	f32 sf14 = m.e[1][1] * m.e[3][2] - m.e[1][2] * m.e[3][1];
	f32 sf15 = m.e[1][1] * m.e[2][2] - m.e[1][2] * m.e[2][1];
	f32 sf16 = m.e[0][1] * m.e[3][2] - m.e[0][2] * m.e[3][1];
	f32 sf17 = m.e[0][1] * m.e[2][2] - m.e[0][2] * m.e[2][1];
	f32 sf18 = m.e[0][1] * m.e[1][2] - m.e[0][2] * m.e[1][1];
	
	result.e[0][0] = +(m.e[1][1] * sf00 - m.e[2][1] * sf01 + m.e[3][1] * sf02);
	result.e[0][1] = -(m.e[0][1] * sf00 - m.e[2][1] * sf03 + m.e[3][1] * sf04);
	result.e[0][2] = +(m.e[0][1] * sf01 - m.e[1][1] * sf03 + m.e[3][1] * sf05);
	result.e[0][3] = -(m.e[0][1] * sf02 - m.e[1][1] * sf04 + m.e[2][1] * sf05);
	
	result.e[1][0] = -(m.e[1][0] * sf00 - m.e[2][0] * sf01 + m.e[3][0] * sf02);
	result.e[1][1] = +(m.e[0][0] * sf00 - m.e[2][0] * sf03 + m.e[3][0] * sf04);
	result.e[1][2] = -(m.e[0][0] * sf01 - m.e[1][0] * sf03 + m.e[3][0] * sf05);
	result.e[1][3] = +(m.e[0][0] * sf02 - m.e[1][0] * sf04 + m.e[2][0] * sf05);
	
	result.e[2][0] = +(m.e[1][0] * sf06 - m.e[2][0] * sf07 + m.e[3][0] * sf08);
	result.e[2][1] = -(m.e[0][0] * sf06 - m.e[2][0] * sf09 + m.e[3][0] * sf10);
	result.e[2][2] = +(m.e[0][0] * sf11 - m.e[1][0] * sf09 + m.e[3][0] * sf12);
	result.e[2][3] = -(m.e[0][0] * sf08 - m.e[1][0] * sf10 + m.e[2][0] * sf12);
	
	result.e[3][0] = -(m.e[1][0] * sf13 - m.e[2][0] * sf14 + m.e[3][0] * sf15);
	result.e[3][1] = +(m.e[0][0] * sf13 - m.e[2][0] * sf16 + m.e[3][0] * sf17);
	result.e[3][2] = -(m.e[0][0] * sf14 - m.e[1][0] * sf16 + m.e[3][0] * sf18);
	result.e[3][3] = +(m.e[0][0] * sf15 - m.e[1][0] * sf17 + m.e[2][0] * sf18);
	
	f32 ood = 1.0f / (m.e[0][0] * result.e[0][0] +
					  m.e[1][0] * result.e[0][1] +
					  m.e[2][0] * result.e[0][2] +
					  m.e[3][0] * result.e[0][3]);
	
	result.e[0][0] *= ood;
	result.e[1][0] *= ood;
	result.e[2][0] *= ood;
	result.e[3][0] *= ood;
	result.e[0][1] *= ood;
	result.e[1][1] *= ood;
	result.e[2][1] *= ood;
	result.e[3][1] *= ood;
	result.e[0][2] *= ood;
	result.e[1][2] *= ood;
	result.e[2][2] *= ood;
	result.e[3][2] *= ood;
	result.e[0][3] *= ood;
	result.e[1][3] *= ood;
	result.e[2][3] *= ood;
	result.e[3][3] *= ood;
	
	
	return result;
}

inline m4 OrthoMatrix(f32 l, f32 b, f32 r, f32 t, f32 n, f32 f)
{
	m4 result = {};
	
	result.e[0][0] = 2.0f / (r - l);
	result.e[1][1] = 2.0f / (t - b);
	result.e[2][2] = -1.0f / (f - n);
	
	result.e[0][3] = (l + r) / (l - r);
	result.e[1][3] = (b + t) / (b - t);
	result.e[2][3] = -n / (f - n);
	
	result.e[3][3] = 1.0f;
	
	return result;
}

inline m4 ProjectionMatrix(f32 aspectRatio)
{
	f32 fov = 74.0f;
	f32 nearPlane = 0.01f;
	//f32 farPlane = 200.0f;
	
	f32 range = Tan(0.5f * fov * (PI / 180.0f)) * nearPlane;
	f32 left = -range * aspectRatio;
	f32 right =  range * aspectRatio;
	f32 bottom = -range;
	f32 top =  range;
	m4 result = {};
	
	result.e[0][0] = (2.0f * nearPlane) / (right - left);
	result.e[1][1] = (2.0f * nearPlane) / (top   - bottom);
	result.e[2][2] = -1.0f;
	result.e[3][2] = -1.0f;
	result.e[2][3] = -2.0f * nearPlane;
	
	return result;
}

// random

inline u32 Xor32(u32 *seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed;
}

inline f32 Xor32f(u32 *seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed * 2.3283064365387e-10f;
}