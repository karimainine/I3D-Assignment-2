#ifndef CONTROLS_H
#define CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/* Struct to handle different controls for debugging */
typedef struct
{
	/* Flags to determine what mode is active */
	bool normals;
	bool axes;
	bool wireframe;
	bool day;
	bool mainCamera;
} Controls;

extern Controls controls;

/* Initialises control states to false */
void initControls(Controls *controls);

  void drawAxes();

#ifdef __cplusplus
}
#endif

#endif
