#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "utils.h"
#include "gl.h"
#include "camera.h"
#include "waves.h"
#include "utils.h"
#include "light.h"
#include "boat.h"
#include "keys.h"
#include "controls.h"
#include "seabed.h"
#include "texture.h"
#include "skybox.h"
#include <string.h>
#include <stdio.h>
	
	typedef struct
	{
		int x;	
		int y;		
	} Screen;
	
	
	
	void drawScene(void);
	void drawLeftScreen(void);
	void drawRightScreen(void);
	void drawAxes(Vec3f pos, Vec3f len);
	void display(void);
	void reshape(int x, int y);
	void idle(void);
	void mouseMove(int x, int y);
	void mouseDown(int button, int state, int x, int y);
	void updateKey(int key, bool state);
	void keyboard(unsigned char key, int x, int y);
	void updateKeySpecial(int key, bool state);
	void keyUp(unsigned char key, int x, int y);
	void keyboardSpecialDown(int key, int x, int y);
	void keyboardSpecialUp(int key, int x, int y);
	void init(void);
	void renderBitmapString(float x, float y, float z, void *font, char *string);
	void printFPS(float x, float y, float z);
	void printOnScreen(float x, float y, float z, char* s);
	void checkCollision(void);
	
#ifdef __cplusplus
}
#endif

#endif