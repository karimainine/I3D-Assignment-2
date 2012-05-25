#ifndef CANNONBALL_H
#define CANNONBALL_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "utils.h"
#include "gl.h"

#define GRAVITY (-9.8)
#define RADIUS 2
#define SLICES 30
#define STACKS 30
#define ANIMATION_TIME 0.0001
#define INIT_FORCE 200
	
	/* The Grid struct is used to hold the grid of vertices representing
	 the waves */
	typedef struct
	{
		Vec3f dir;
		Vec3f pos;
		Vec3f v;
		float previousTime;
		
	} CannonBall;
	
	void drawBall(CannonBall *ball);
	void updateBall(CannonBall *ball);
	
#ifdef __cplusplus
}
#endif

#endif
