#include <stdlib.h>
#include "skybox.h"
#include "texture.h"
#include "gl.h"

static GLuint topTexture;
static GLuint leftTexture;
static GLuint rightTexture;
static GLuint frontTexture;
static GLuint backTexture;

void initSky(Sky *sky, int size){
	
	topTexture = texture_load("textures/top.jpg");
	leftTexture = texture_load("textures/left.jpg");
	rightTexture = texture_load("textures/right.jpg");
	frontTexture = texture_load("textures/front.jpg");
	backTexture = texture_load("textures/back.jpg");
	
	sky->startX = sky->startY = sky->startZ = -0.5*size;
	sky->endX = sky->endY = sky->endZ = 0.5*size;
	
}

void drawSky(Sky *sky, Boat *boat){
	// Store the current matrix
	glPushMatrix();
	glLoadIdentity();
	
	gluLookAt(0, 0, 0, boat->roll, 0, boat->pitch, 0, 1, 0);

	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	
	static float diffuse[] = {0, 1, 1, 1};
	static float ambient[] = {0, 1, 1, 1};
	static float specular[] = {1, 1, 1, 1};
	static float shininess = 0.0f;
		
	/* Apply the material, this will interact with the light to
	 produce the final colour */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	// Render the front quad
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(sky->startX, sky->startY, sky->endZ);
	glTexCoord2f(1, 0); glVertex3f(sky->endX, sky->startY, sky->endZ);
	glTexCoord2f(1, 1); glVertex3f(sky->endX,  sky->endY, sky->endZ);
	glTexCoord2f(0, 1); glVertex3f(sky->startX,  sky->endY, sky->endZ);
	glEnd();
	
	// Render the left quad
	glBindTexture(GL_TEXTURE_2D, leftTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(sky->endX, sky->startY, sky->startZ);
	glTexCoord2f(1, 0); glVertex3f(sky->endX, sky->startY, sky->endZ);
	glTexCoord2f(1, 1); glVertex3f(sky->endX, sky->endY, sky->endZ);
	glTexCoord2f(0, 1); glVertex3f(sky->endX, sky->endY, sky->startZ);
	glEnd();
	
	// Render the back quad
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(sky->startX, sky->startY, sky->startZ);
	glTexCoord2f(1, 0); glVertex3f(sky->endX, sky->startY, sky->startZ);
	glTexCoord2f(1, 1); glVertex3f(sky->endX, sky->endY, sky->startZ);
	glTexCoord2f(0, 1); glVertex3f(sky->startX, sky->endY, sky->startZ);
	
	glEnd();
	
	// Render the right quad
	glBindTexture(GL_TEXTURE_2D, rightTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(sky->startX, sky->startY, sky->endZ);
	glTexCoord2f(1, 0); glVertex3f(sky->startX, sky->startY, sky->startZ);
	glTexCoord2f(1, 1); glVertex3f(sky->startX, sky->endY, sky->startZ);
	glTexCoord2f(0, 1); glVertex3f(sky->startX, sky->endY, sky->endZ);
	glEnd();
	
	// Render the top quad
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(sky->startX, sky->endY, sky->startZ);
	glTexCoord2f(0, 0); glVertex3f(sky->startX, sky->endY, sky->endZ);
	glTexCoord2f(1, 0); glVertex3f(sky->endX, sky->endY, sky->endZ);
	glTexCoord2f(1, 1); glVertex3f(sky->endX, sky->endY, sky->startZ);
	glEnd();
	
	// Restore enable bits and matrix
	glPopAttrib();
	glPopMatrix();
}