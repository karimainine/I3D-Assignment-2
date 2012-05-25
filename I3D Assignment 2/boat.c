#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "boat.h"
#include "controls.h"
#include "waves.h"
#include "gl.h"

#ifndef min
#define min(a, b) (a <= b ? a : b)
#endif

#ifndef max
#define max(a, b) (a >= b ? a : b)
#endif

#ifndef clamp
#define clamp(val, a, b) (val > a ? a : val < b ? : b : val)
#endif

#define myCrossPlatformMin(a, b) ((a)<(b)?(a):(b))
#define sign(val) ((val)>0?1:-1)

/* Initialises the given boat with default values, and loads the obj
   file given by the filename */
void initBoat(Boat *boat, const char *meshFilename, Vec3f position)
{
	boat->radius = BOAT_RADIUS;
	boat->pos = position;
	boat->heading = 90; /* The gallon obj faces this direction initially */
	boat->speed = 0;
	boat->maxSpeed = 5.0;
	boat->acceleration = 3.0;
	boat->turningAcceleration = 50.0;
	boat->turnVelocity = 0;
	boat->displacement = -0.4;
	boat->roll = 0;
	boat->pitch = 0;
	
	boat->nBalls = 0;
	boat->balls = malloc(MAX_CANNON_BALLS * sizeof(CannonBall));
	
	boat->mesh = objMeshLoad(meshFilename);
}

/* Draws the obj mesh */
void drawMesh(OBJMesh *mesh)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->numIndices; ++i)
	{
		unsigned int index = mesh->indices[i];
		float* vert = (float*)((char*)mesh->vertices + index * mesh->stride);
		float* norm = (float*)((char*)vert + mesh->normalOffset);
		if (mesh->hasNormals)
			glNormal3fv(norm);
		glVertex3fv(vert);
	}
	glEnd();
}

/* Draws the given boat */
void drawBoat(Boat *boat, float* ambient)
{
	/* Draw the boat as red to contrast with the waves */
	static float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//static float ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	static float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float shininess = 256.0f;

	glPushMatrix();

	glTranslatef(boat->pos.x, boat->pos.y, boat->pos.z);
	
	
	if (controls.axes)
		drawAxes(cVec3f(0, 0, 0), cVec3f(10, 10, 10));
	
	glRotatef(boat->roll, 0, 0, -1);
	glRotatef(boat->pitch, 1, 0, 0);
	glRotatef(boat->heading, 0, 1, 0);

	if (controls.axes)
		drawAxes(cVec3f(0, 0, 0), cVec3f(10, 10, 10));

	/* The boat is a little big */
	glScalef(0.1, 0.1, 0.1);
	glPushMatrix();
	glRotatef(-90, 0, 1, 0); /* To account for the initial heading */

	/* Apply the material, this will interact with the light to
	   produce the final colour */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	drawMesh(boat->mesh);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

/* Updates the boat's position based on movement flags */
void updateBoat(Boat *boat, bool up, bool down, bool left, bool right, float dt, bool *fireleft, bool *fireright)
{
	Vec4f v;
	Vec2f headingDir;

	/* Check flag states and add or sbtract from the forward/turn
	   rate of the boat */
	if (up) boat->speed += boat->acceleration * dt;
	if (down) boat->speed -= boat->acceleration * dt;

	/* Limit the boat speed */
	boat->speed = clamp(boat->speed, 0, boat->maxSpeed);
	
	/* Accelerate the turning velocity of the boat when a key is down */
	if (left) boat->turnVelocity += boat->turningAcceleration * dt;
	if (right) boat->turnVelocity -= boat->turningAcceleration * dt;

	/* Limit turning speed relative to 5x boat speed in degrees
	   per second */
	boat->turnVelocity = clamp(boat->turnVelocity, -fabs(boat->speed) * 5.0, fabs(boat->speed) * 5.0);

	/* Apply turning velocity damping (so the ship stops turning
	   when the button is released) */
	boat->turnVelocity -= myCrossPlatformMin(fabs(boat->turnVelocity), 0.5) * sign(boat->turnVelocity);
	
	/* Calculate the heading direction vector of the boat */
	headingDir.x = sinf(boat->heading * M_PI / 180.0);
	headingDir.y = cosf(boat->heading * M_PI / 180.0);

	/* Move the ship forwards at its current speed */
	boat->heading += boat->turnVelocity * dt;
	boat->pos.x += headingDir.x * boat->speed * dt;
	boat->pos.z += headingDir.y * boat->speed * dt;

	/* Orient the boat appropriately given its current location in
	   the waves */
	v = calcSineValue(boat->pos.x, boat->pos.z);
	boat->pos.y = v.w - boat->displacement;
	boat->roll = asin(v.x) * 180.0 / M_PI;
	boat->pitch = asin(v.z) * 180.0 / M_PI;
	
	if(*fireleft){
		*fireleft = false;
		initBall(boat, true);
	}
	
	if(*fireright){
		*fireright = false;
		initBall(boat, false);
	}
}

void initBall(Boat *boat, bool left){
	CannonBall ball;
	
	if(boat->nBalls < MAX_CANNON_BALLS){
		ball.pos = boat->pos;
		ball.previousTime = glutGet(GLUT_ELAPSED_TIME);
		
		if(left){
			ball.dir.z = boat->heading + 90;
		}else{
			ball.dir.z = boat->heading - 90;
		}
		ball.dir.x = sinf(ball.dir.z * M_PI/180.0);
		ball.dir.y = cosf(ball.dir.z * M_PI/180.0);
		
		ball.v.x = ball.dir.x * INIT_FORCE;
		ball.v.y = INIT_FORCE;
		ball.v.z = ball.dir.y * INIT_FORCE;
		
		boat->balls[boat->nBalls] = ball;
		boat->nBalls++;
	}
}

void drawAllBalls(Boat *boat){
	int i;
	for (i=0; i<boat->nBalls; i++){
		drawBall(&(boat->balls[i]));
	}
}

void updateAllBalls(Boat *boat){
	int i;
	for (i=0; i<boat->nBalls; i++){
		updateBall(&(boat->balls[i]));
	}
}

bool boatsCollided(Boat *boat1, Boat *boat2){
	float distance;
	distance = getDistanceDiff(boat1->pos, boat1->pos);
	if(distance < (boat1->radius + boat2->radius)){
		return true;
	}
	return false;
}

