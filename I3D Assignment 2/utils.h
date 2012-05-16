#pragma once

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Bools are handy, we should have some */
#if WIN32
#define bool int
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif

/* Some handy macros */
#ifndef WIN32
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(x, a, b) min(max(x, a), b)

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

/* Struct for a vector with 2 components */
typedef struct
{
	float x, y;
} Vec2f;

/* Struct for a vector with 3 components */
typedef struct
{
	float x, y, z;
} Vec3f;

/* Struct for a vector with 4 components */
typedef struct
{
	float x, y, z, w;
} Vec4f;

/* Constructor for a Vec3f struct */
Vec3f cVec3f(float x, float y, float z);

/* Constructor for a Vec4f struct */
Vec4f cVec4f(float x, float y, float z, float w);

#ifdef __cplusplus
}
#endif

#endif
