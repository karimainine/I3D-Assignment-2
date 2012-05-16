#include "camera.h"
#include "gl.h"

/* Initialises given camera with some default values */
void initCamera(Camera *camera)
{
	camera->fov = 75.0f;
	camera->clipNear = 0.1f;
	camera->clipFar = 200.0f;
	camera->rotating = false;
	camera->zooming = false;
	camera->zoom = 0.0f;
	camera->sensitivity = 0.4f;
	camera->pos.x = 0.0f;
	camera->pos.y = 0.0f;
	camera->pos.z = 10.0f;
	camera->rot.x = 0.0f;
	camera->rot.y = 0.0f;
	camera->rot.z = 0.0f;
}

/* Transforms the scene so that it will be seen based on the camera's position */
void setupCamera(Camera *camera)
{
	glTranslatef(0, 0, -camera->zoom);
	glTranslatef(-camera->pos.x, -camera->pos.y, -camera->pos.z);
	glRotatef(camera->rot.x, 1, 0, 0);
	glRotatef(camera->rot.y, 0, 1, 0);
}
