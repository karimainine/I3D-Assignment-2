#include "controls.h"

/* Initialises control states to false */
void initControls(Controls *controls)
{
	controls->axes = false;
	controls->normals = false;
	controls->wireframe = false;
	controls->day = true;
	controls->mainCamera = true;
}
