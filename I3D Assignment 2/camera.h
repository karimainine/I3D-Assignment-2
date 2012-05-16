#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/* The Camera struct is used for viewing the contents of the scene */
typedef struct
{
	float fov;		/* Field of view of the camera, in degrees, in the y direction */
	float clipNear;		/* Depth value of the near clipping plane (not position on z axis!) */
	float clipFar;		/* Depth value of the far clipping plane (not position on z axis!) */
	bool rotating;		/* Flag to determine if the camera is rotating */
	bool zooming;		/* Flag to determine if camera is zooming */
	float zoom;		/* Distance camera is zoomed in/out */
	float sensitivity;	/* Speed of camera rotation/zoom */
	Vec3f pos;		/* Position of the camera */
	Vec3f rot;		/* Rotation of the camera */
} Camera;

/* Initialises given camera with some default values */
void initCamera(Camera *camera);

/* Transforms the scene so that it will be seen based on the camera's position */
void setupCamera(Camera *camera);

#ifdef __cplusplus
}
#endif

#endif
