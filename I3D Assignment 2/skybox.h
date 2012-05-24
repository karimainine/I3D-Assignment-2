#ifndef SKYBOX_H
#define SKYBOX_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "boat.h"
#include "utils.h"
	
	typedef struct{
		float startX;
		float startY;
		float startZ;
		float endX;
		float endY;
		float endZ;
	}Sky;
	
	void drawSky(Sky*, Boat*);
	void initSky(Sky*, int);
	
#ifdef __cplusplus
}
#endif

#endif