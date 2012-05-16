#include "utils.h"

/* Constructor for a Vec3f struct */
Vec3f cVec3f(float x, float y, float z)
{
	Vec3f v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/* Constructor for a Vec4f struct */
Vec4f cVec4f(float x, float y, float z, float w)
{
	Vec4f v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}
