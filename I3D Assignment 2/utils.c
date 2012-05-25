#include "utils.h"
#include <math.h>

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

float getDistanceDiff(Vec3f a, Vec3f b){
	return sqrt((pow((a.x - b.x),2)) + (pow((a.y - b.y),2)) + (pow((a.z - b.z),2)));
}
