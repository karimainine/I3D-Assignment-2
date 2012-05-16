#ifndef LIGHT_H
#define LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/* Struct to hold the position and colour information of a Light */
typedef struct
{
	Vec4f position;		/* Position of the light */
	Vec4f ambient;		/* Ambient colour of the light */
	Vec4f diffuse;		/* Diffuse colour of the light */
	Vec4f specular;		/* Specular colour of the light */
	float shininess;	/* Shininess of the light's specularity (higher values will give smaller bright patches) */
} Light;

/* Applies the given arguments to the light struct */
void initLight(Light *light, Vec4f position, Vec4f ambient, Vec4f diffuse, Vec4f specular, float shininess);

/* Applies the values stored in light to GL_LIGHT0 */
void setupLight(Light *light);

#ifdef __cplusplus
}
#endif

#endif
