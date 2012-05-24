#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "utils.h"

	/* The Screen struct is used for creating a screen/window for each player */
	typedef struct
	{
		int x;	
		int y;		
	} Screen;
	
	void drawScreen();
	
#ifdef __cplusplus
}
#endif

#endif