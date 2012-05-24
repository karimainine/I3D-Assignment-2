#include <stdlib.h>
#include "seabed.h"
#include "gl.h"

void drawSeaBed(){
	glBegin(GL_QUADS);
	//glTexCoord2f(0, 0);
	glVertex2f(-1, -1);
	//glTexCoord2f(1, 0);
	glVertex2f(1, -1);
	//glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	//glTexCoord2f(0, 1);
	glVertex2f(-1, 1);
	glEnd();
}