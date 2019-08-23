
#pragma once

#include <stdint.h>

#define BONE_COUNT 16

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef size_t uSize;

#if defined _WIN64
typedef i64 iSize;
#elif defined _WIN32
typedef i32 iSize;
#elif defined __x86_64_ && !defined __ILP32__
typedef i64 iSize;
#else
typedef i32 iSize;
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

union v2 {
	struct {
		f32 x, y;
	};
	f32 e[2];
};

union v2i {
	struct {
		i32 x, y;
	};
	i32 e[2];
};

union v3 {
	struct { f32 x, y, z; };
	struct { f32 r, g, b; };
	f32 e[3];
};

union v4 {
	struct { f32 x, y, z, w; };
	struct { f32 r, g, b, a; };
	f32 e[4];
};

struct m2
{
	f32 e[2][2];
};

struct m3
{
	f32 e[3][3];
};

struct m4
{
	f32 e[4][4];
};