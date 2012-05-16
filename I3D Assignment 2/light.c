#include "light.h"
#include "gl.h"

/* Applies the given arguments to the light struct */
void initLight(Light *light, Vec4f position, Vec4f ambient, Vec4f diffuse, Vec4f specular, float shininess)
{
	light->position = position;
	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;
	light->shininess = shininess;
}

/* Applies the values stored in light to GL_LIGHT0 */
void setupLight(Light *light)
{
	glLightfv(GL_LIGHT0, GL_POSITION, (const GLfloat *) &(light->position));
	glLightfv(GL_LIGHT0, GL_AMBIENT, (const GLfloat *) &(light->ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (const GLfloat *) &(light->diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, (const GLfloat *) &(light->specular));
	glLightf(GL_LIGHT0, GL_SHININESS, light->shininess);
}
