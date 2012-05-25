#include "cannon_ball.h"
#include <stdio.h>

void drawBall(CannonBall *ball){
	static float diffuse[] = { 1.0f, 1.0f, 1.10f, 1.0f };
	static float ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float shininess = 256.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	glPushMatrix();
	glTranslatef(ball->pos.x, ball->pos.y, ball->pos.z);
	glRotatef(10.0f, 0, 1, 0); 
	glutSolidSphere(RADIUS, SLICES, STACKS);
	glPopMatrix();
}

void updateBall(CannonBall *ball){
	float now = glutGet(GLUT_ELAPSED_TIME);
	float dt = (now - ball->previousTime)*ANIMATION_TIME;
	
	ball->v.x = ball->dir.x * INIT_FORCE;
	ball->v.y += (dt*GRAVITY) * INIT_FORCE;
	ball->v.z = ball->dir.y * INIT_FORCE;
	
	ball->pos.x += ball->v.x*dt;
	ball->pos.y += ball->v.y*dt;
	ball->pos.z += ball->v.z*dt;
	
	ball->previousTime = now;
	
}
